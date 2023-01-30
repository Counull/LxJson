#include "lept_context.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stack>
#include <string.h>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vcruntime.h>
#include <vector>

using namespace std::literals;
using namespace LxJson;
lept_context::lept_context(const std::string_view& json)
    : json(json)
{
}

lept_context::~lept_context()
{
}
void lept_context::parse_whithspace()
{
    for (auto& c : json) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            break;
        }

        json.remove_prefix(1);
    }
}

lept_result lept_context::parse_value(lept_value& value)
{
    if (json.empty()) {
        return lept_result::LEPT_PARSE_EXPECT_VALUE;
    }

    switch (json.front()) {
    case 'n':
        return parse_expr<lept_value::NULL_EXPR>(value);
    case 't':
        return parse_expr<lept_value::TRUE>(value);
    case 'f':
        return parse_expr<lept_value::FALSE>(value);
    case '\"':
        return parse_string(value);
    case '[':
        return parse_array(value);
    case '{':
        return parse_object(value);
    default:
        return parse_number(value);
    }
}

template <const char* E>
lept_result lept_context::parse_expr(lept_value& value)
{
    const auto STRLEN = strlen(E);
    if (json.length() < STRLEN) {
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }

    for (size_t i = 0; i < STRLEN; i++) {
        if (E[i] != json[i]) {
            return lept_result::LEPT_PARSE_INVALID_VALUE;
        }
    }

    if constexpr (E == lept_value::NULL_EXPR) {
        value.setValue(nullptr);
    } else if constexpr (E == lept_value::TRUE) {
        value.setValue(true);
    } else if constexpr (E == lept_value::FALSE) {

        value.setValue(false);
    }
    json.remove_prefix(STRLEN);
    return lept_result::LEPT_PARSE_OK;
}

lept_result LxJson::lept_context::parse_number(lept_value& value)
{

    auto p = json.data();
    if (*p == '-')
        p++;
    if (*p == '0')
        p++;
    else {
        if (*p == '0' || !isdigit(*p))
            return lept_result::LEPT_PARSE_INVALID_VALUE;
        for (p++; isdigit(*p); p++)
            ;
    }
    if (*p == '.') {
        p++;
        if (!isdigit(*p))
            return lept_result::LEPT_PARSE_INVALID_VALUE;
        for (p++; isdigit(*p); p++)
            ;
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-')
            p++;
        if (!isdigit(*p))
            return lept_result::LEPT_PARSE_INVALID_VALUE;
        for (p++; isdigit(*p); p++)
            ;
    }

    double d;
    auto ret = std::from_chars(json.data(), p, d);

    if (ret.ec != std::errc {}) {
        if (ret.ec == std::errc::result_out_of_range && (d == HUGE_VAL || d == -HUGE_VAL)) {
            return lept_result::LEPT_PARSE_NUMBER_TOO_BIG;
        }

        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }

    value.setValue(d);

    auto offset = p - json.data();
    json.remove_prefix(offset);
    return lept_result::LEPT_PARSE_OK;
}

lept_result LxJson::lept_context::parse_string(lept_value& value)
{
    std::string retStr;
    auto ret = parse_std_string(retStr);
    if (ret == lept_result::LEPT_PARSE_OK) {
        value.setValue(std ::move(retStr));
    }
    return ret;
}

lept_result LxJson::lept_context::parse_std_string(std::string& strOut)
{
    assert(json.front() == '\"');
    size_t p = 1;
    auto begin = ++json.begin();
    std::unique_ptr<std::string> strBuffer;

    for (auto iter = begin; iter != json.end(); iter++) {
        char escape = '\0';
        p++;
        switch (*iter) {
        case '\\':
            p++;

            switch (*++iter) {
            case '\"':
                escape = '\"';
                break;
            case '\\':
                escape = '\\';
                break;
            case '/':
                escape = '/';
                break;
            case 'b':
                escape = '\b';
                break;
            case 'f':
                escape = '\f';
                break;
            case 'n':
                escape = '\n';
                break;
            case 'r':
                escape = '\r';
                break;
            case 't':
                escape = '\t';
                break;
            default:
                return lept_result::LEPT_PARSE_INVALID_STRING_ESCAPE;
            }
            break;
        case '\"':
            if (!strBuffer) {
                strBuffer = std::make_unique<std::string>(std::string { begin, iter });
            }

            strOut = std::move(*strBuffer);
            json.remove_prefix(p);
            return lept_result::LEPT_PARSE_OK;
        }
        if (!strBuffer) {
            strBuffer = std::make_unique<std::string>(std::string { begin, iter });
        }
        if (strBuffer) {
            if (escape != '\0') {
                strBuffer->push_back(escape);
                escape = '\0';
            } else {
                strBuffer->push_back(*iter);
            }
        }
    }

    return lept_result::LEPT_PARSE_MISS_QUOTATION_MARK;
}

lept_result LxJson::lept_context::parse_array(lept_value& value)
{
    assert(json.front() == '[');
    json.remove_prefix(1);
    std::vector<lept_value> finalArr;

    while (json.length() > 0) {

        parse_whithspace();
        auto c = json.front();
        if (c == ']') {
            json.remove_prefix(1);
            value.setValue(finalArr);
            return lept_result::LEPT_PARSE_OK;
        } else if (c == ',') {
            json.remove_prefix(1);
            parse_whithspace();
        }

        lept_value val;
        if (auto ret = parse_value(val); ret != lept_result::LEPT_PARSE_OK) {
            return ret;
        }
        finalArr.emplace_back(std::move(val));
    }
    return lept_result::LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
}

lept_result LxJson::lept_context::parse_object(lept_value& value)
{
    assert(json.front() == '{');
    json.remove_prefix(1);
    parse_whithspace();

    JsonObject map;
    if (!json.empty() && json.front() == '}') {
        json.remove_prefix(1);
        value.setValue(map);
        return lept_result::LEPT_PARSE_OK;
    }

    while (json.length() > 0) {

        if (json.front() != '\"') {
            return lept_result::LEPT_PARSE_MISS_KEY;
            break;
        }
        std::string key;

        if (auto ret = parse_std_string(key); ret != lept_result::LEPT_PARSE_OK) {
            break;
        }
        parse_whithspace();

        if (json.empty() || json.front() != ':') {
            return lept_result::LEPT_PARSE_MISS_COLON;
            break;
        }
        json.remove_prefix(1);
        parse_whithspace();
        lept_value val;
        if (auto ret = parse_value(val); ret != lept_result::LEPT_PARSE_OK) {
            break;
        }
        map.emplace(std::move(key), std::move(val));

        parse_whithspace();
        if (json.empty()) {
            return lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
        if (json.front() == '}') {
            json.remove_prefix(1);
            value.setValue(map);
            return lept_result::LEPT_PARSE_OK;
        } else if (json.front() == ',') {
            json.remove_prefix(1);
            parse_whithspace();
            if (json.empty()) {
                return lept_result::LEPT_PARSE_MISS_KEY;
            }
        } else {
            return lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }

    return lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
}

#include "lept_context.h"

#include <cctype>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stack>
#include <string.h>
#include <string_view>
#include <system_error>
#include <vcruntime.h>

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
        return parse_expr<NULL_EXPR>(value);
        break;
    case 't':
        return parse_expr<TRUE>(value);
        break;
    case 'f':
        return parse_expr<FALSE>(value);
        break;
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

    if constexpr (E == NULL_EXPR) {
        value.setValue(nullptr);
    } else if constexpr (E == TRUE) {
        value.setValue(true);
    } else if constexpr (E == FALSE) {

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
    assert(json.front() == '\"');
    std::string_view view(json);
    view.remove_prefix(1);
    size_t p = 0;
    for (auto c : json) {

        switch (c) {

        case '\"':
            view.remove_suffix(view.length() - p);
            value.setValue(view.data());
            json.remove_prefix(++p);
            return lept_result::LEPT_PARSE_OK;
        case '\0':
            return lept_result::LEPT_PARSE_MISS_QUOTATION_MARK;
        }
        ++p;
    }
}

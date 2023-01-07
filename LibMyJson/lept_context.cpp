#include "lept_context.h"
#include <charconv>
#include <cstddef>
#include <ctype.h>
#include <iostream>
#include <string.h>
#include <string_view>
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
        value.setType(lept_type::LEPT_NULL);
    } else if constexpr (E == TRUE) {
        value.setType(lept_type::LEPT_TRUE);
    } else if constexpr (E == FALSE) {
        value.setType(lept_type::LEPT_FALSE);
    }
    json.remove_prefix(STRLEN);
    return lept_result::LEPT_PARSE_OK;
}

lept_result LxJson::lept_context::parse_number(lept_value& value)
{

    if (json.starts_with('.') || json.starts_with('+')
        || isalpha(json.front())) {
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }

    double d;
    size_t validOffset = 0;
    for (size_t i = 0; i < json.size(); i++) {
        auto c = json[i];
        if (!isdigit(c) && c != '.' && c != '-' && c != '+') {
            if (!isalpha(c) || toupper(c) != 'E') {
                break;
            }
        }
        ++validOffset;
    }

    if (json[validOffset - 1] == '.') {
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }
    auto ret = std::from_chars(json.data(), json.data() + validOffset, d);
    if (ret.ec == std::errc::invalid_argument) {
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }

    value.setNum(d);
    value.setType(lept_type::LEPT_NUMBER);
    json.remove_prefix(validOffset);
    return lept_result::LEPT_PARSE_OK;
}

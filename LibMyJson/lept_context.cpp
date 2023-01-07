#include "lept_context.h"
#include <memory>
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
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }
}

template <const char* E>
lept_result lept_context::parse_expr(lept_value& value)
{
    const auto STRLEN = strlen(E);
    if (json.length() < STRLEN) {
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }

    auto valIntr = json.cbegin();
    for (size_t i = 0; i < STRLEN; i++) {

        if (E[i] != *valIntr) {
            return lept_result::LEPT_PARSE_INVALID_VALUE;
        }
        valIntr++;
    }
    json.remove_prefix(STRLEN);
    if constexpr (E == NULL_EXPR) {
        value.setType(lept_type::LEPT_NULL);
    } else if constexpr (E == TRUE) {
        value.setType(lept_type::LEPT_TRUE);
    } else if constexpr (E == FALSE) {
        value.setType(lept_type::LEPT_FALSE);
    }

    return lept_result::LEPT_PARSE_OK;
}

#include "lept_context.h"
#include <memory>
#include <string_view>
#include <vcruntime.h>

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
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            json.remove_prefix(1);
        } else {
            break;
        }
    }
}

lept_result lept_context::parse_value(lept_value& value)
{
    switch (json.front()) {
    case 'n':
        return parse_null(value);
    case '\0':
        return lept_result::LEPT_PARSE_EXPECT_VALUE;
    default:
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }
    
}

lept_result lept_context::parse_null(lept_value& value)
{
    bool isNull = false;

    if (json[0] != 'n' || json[1] != 'u' || json[2] != 'l' || json[3] != 'l') {
        return lept_result::LEPT_PARSE_INVALID_VALUE;
    }

    json.remove_prefix(GetLengthOfBaseJasonStr("null"));
    value.setType(lept_type::LEPT_NULL);
    return lept_result::LEPT_PARSE_OK;
}

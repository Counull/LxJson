#include "lept_context.h"
#include <memory>
#include <string_view>
#include <vcruntime.h>

lept_context::lept_context(const std::string& json)
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

lept_result lept_context::parse_value(const lept_value& value) {
    
}

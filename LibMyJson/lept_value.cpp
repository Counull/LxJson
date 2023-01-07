#include "lept_value.h"
#include "lept_context.h"
#include <iostream>
namespace LxJson {

lept_value::lept_value()
{
}

lept_value::~lept_value()
{
}

lept_result lept_value::parse(const std::string& json)
{
    lept_context context(json);
    setType(lept_type::LEPT_NULL);
    context.parse_whithspace();
    auto ret = context.parse_value(*this);
    if (ret == lept_result::LEPT_PARSE_OK) {
        context.parse_whithspace();
        if (!context.isEmpty()) {
            ret = lept_result::LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}
}
#include "lept_value.h"
#include "lept_context.h"
#include <string>
namespace LxJson {

lept_value::lept_value()
{
}

lept_value::~lept_value()
{
}

lept_result lept_value::parse(const std::string& json)
{
    auto str = std::string(json + " "); // string view 不是\0结尾导致了灾难性的语句
    lept_context context(str);
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
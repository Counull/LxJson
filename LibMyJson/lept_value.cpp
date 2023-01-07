#include "lept_value.h"
#include "lept_context.h"
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

    return context.parse_value(*this);

}

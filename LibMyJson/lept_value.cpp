#include "lept_value.h"
#include "lept_context.h"
#include "lept_stringify.h"
#include <exception>
#include <iostream>

#include <string>
namespace LxJson {

lept_value::lept_value()
{
}

lept_value::~lept_value()
{
}

bool operator==(const lept_value& lhs, const lept_value& rhs)
{
    auto lhsType = lhs.getType();
    if (lhsType != rhs.getType()) {
        return false;
    }

    switch (lhsType) {

    case lept_type::LEPT_BOOLEAN:
        return lhs.getValue<lept_type::LEPT_BOOLEAN>() == rhs.getValue<lept_type::LEPT_BOOLEAN>();
    case lept_type::LEPT_NULL:
        return lhs.getValue<lept_type::LEPT_NULL>() == rhs.getValue<lept_type::LEPT_NULL>();
    case lept_type::LEPT_NUMBER:
        return lhs.getValue<lept_type::LEPT_NUMBER>() == rhs.getValue<lept_type::LEPT_NUMBER>();
    case lept_type::LEPT_STRING:
        return lhs.getValue<lept_type::LEPT_STRING>() == rhs.getValue<lept_type::LEPT_STRING>();
    case lept_type::LEPT_ARRAY:
        return lhs.getValue<lept_type::LEPT_ARRAY>() == rhs.getValue<lept_type::LEPT_ARRAY>();

        /* case lept_type::LEPT_OBJECT:
             return lhs.getValue<lept_type::LEPT_OBJECT>() == rhs.getValue<lept_type::LEPT_OBJECT>(); */
    default:
        throw std::exception("Json type not valid.");
        break;
    }
}

lept_result lept_value::parse(const std::string& json)
{

    lept_context context(json);

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

lept_result lept_value::stringify(std::string& jsonOut)
{
    lept_stringify stringify;
    if (auto ret = stringify.stringify(*this); ret != lept_result::LEPT_STRINGIFY_OK) {
        return ret;
    }
    std::cout << *stringify.json << "***********" << std::endl;
    jsonOut = std::move(*stringify.json);
    std::cout << jsonOut << std::endl;
    return lept_result::LEPT_STRINGIFY_OK;
}

}
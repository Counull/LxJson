#include "lept_stringify.h"
#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
using namespace LxJson;
lept_stringify::lept_stringify()
{
}

lept_stringify::~lept_stringify()
{
}
lept_result LxJson::lept_stringify::stringify(lept_value& value)
{
    if (auto ret = stringify_value(value); ret != lept_result::LEPT_STRINGIFY_OK) {
        return ret;
    }
    return lept_result::LEPT_STRINGIFY_OK;
}

// 我应该对这个引用参数使用Optional而不是这个成员变量
lept_result LxJson::lept_stringify::stringify_value(lept_value& value)
{
    switch (value.getType()) {
    case lept_type::LEPT_BOOLEAN:
        json.emplace(value.getValue<lept_type::LEPT_BOOLEAN>()
                ? lept_value::TRUE
                : lept_value::FALSE);
        break;
    case lept_type::LEPT_NULL:
        json.emplace(lept_value::NULL_EXPR);
        break;
    case lept_type::LEPT_NUMBER:
        json.emplace(std::to_string(value.getValue<lept_type::LEPT_NUMBER>()));
        break;
    case lept_type::LEPT_STRING:
        std::string str = value.getValue<lept_type::LEPT_STRING>();
        json.emplace();
        json->reserve(str.length() + 2);
        json->append("\"");
        json->append(str);
        json->append("\"");

        break;
    }

    return lept_result::LEPT_STRINGIFY_OK;
}

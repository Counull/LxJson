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
lept_result LxJson::lept_stringify::stringify_value(const lept_value& value)
{
    switch (value.getType()) {
    case lept_type::LEPT_BOOLEAN:
        json.append(value.getValue<lept_type::LEPT_BOOLEAN>()
                ? lept_value::TRUE
                : lept_value::FALSE);
        break;
    case lept_type::LEPT_NULL:
        json.append(lept_value::NULL_EXPR);
        break;
    case lept_type::LEPT_NUMBER:
        json.append(std::to_string(value.getValue<lept_type::LEPT_NUMBER>()));
        break;
    case lept_type::LEPT_STRING: {
        std::string str = value.getValue<lept_type::LEPT_STRING>();
        json.append(stringify_string(str));
        break;
    }
    case lept_type::LEPT_ARRAY: {
        json += '[';
        auto array = value.getValue<lept_type::LEPT_ARRAY>();
        for (int i = 0; i < array.size(); i++) {
            if (i > 0) {
                json += ',';
            }
            stringify_value(array[i]);
        }
        json += ']';
        break;
    }
    case lept_type::LEPT_OBJECT: {
        json += '{';
        auto map = value.getValue<lept_type::LEPT_OBJECT>();
        int i = 0;
        for (const auto& pair : map) {
            if (i > 0) {
                json += ',';
            }

            json += stringify_string(pair.first);
            json += ':';
            stringify_value(pair.second);
            ++i;
        }
        json += '}';
    } break;
    }

    return lept_result::LEPT_STRINGIFY_OK;
}

std::string LxJson::lept_stringify::stringify_string(const std::string& val)
{
    std::string ret("\"");
    for (auto& c : val) {
        switch (c) {
        case '\"':
            ret.append(R"(\")");
            break;
        case '\\':
            ret.append(R"(\\)");
            break;
        case '\b':
            ret.append(R"(\b)");
            break;
        case '\f':
            ret.append(R"(\f)");
            break;
        case '\n':
            ret.append(R"(\n)");
            break;
        case '\r':
            ret.append(R"(\r)");
            break;
        case '\t':
            ret.append(R"(\t)");
            break;
        default:
            ret += c;
            break;
        }
    }

    ret += '\"';
    return ret;
}

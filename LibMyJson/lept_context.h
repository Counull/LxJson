#ifndef LEPT_CONTEXT_H
#define LEPT_CONTEXT_H

#pragma once
#include "lept_value.h"
#include <cassert>
#include <coroutine>
#include <string_view>

namespace LxJson {

class lept_context {
public:
    lept_context(const std::string_view& json);
    ~lept_context();

    void parse_whithspace();
    lept_result parse_value(lept_value& value);

    constexpr bool isEmpty() noexcept;

    //  static constexpr std::string_view NULL_EXPR = std::string_view("null");

private:
    std::string_view json;

    template <const char* E>
    lept_result parse_expr(lept_value& value);
    lept_result parse_number(lept_value& value);
    lept_result parse_string(lept_value& value);
    lept_result parse_std_string(std::string& strOut);
    lept_result parse_array(lept_value& value);
    lept_result parse_object(lept_value& value);
};

constexpr bool lept_context::isEmpty() noexcept
{
    return json.empty();
}

}
#endif
#ifndef LEPT_CONTEXT_H
#define LEPT_CONTEXT_H
#include <vcruntime.h>
#pragma once

#include "lept_value.h"
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
    static constexpr char TRUE[] = "true";
    static constexpr char FALSE[] = "false";
    static constexpr char NULL_EXPR[] = "null";

private:
    std::string_view json;

    template <const char* E>
    lept_result parse_expr(lept_value& value);
    lept_result parse_number(lept_value& value);
};

constexpr bool lept_context::isEmpty() noexcept
{
    return json.empty();
}

}
#endif
#ifndef LEPT_CONTEXT_H
#define LEPT_CONTEXT_H

#include <memory>
#include <string_view>
#pragma once
#include "lept_value.h"
#include <string>
class lept_context {
public:
    lept_context(const std::string& json);
    ~lept_context();

    void parse_whithspace();
    lept_result parse_value(const lept_value& value);
    lept_result parse_null(const lept_value& value);
     private : std::string_view json;
};

#endif
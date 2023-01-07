#ifndef LEPT_CONTEXT_H
#define LEPT_CONTEXT_H
#include <cstring>
#include <vcruntime.h>
#pragma once

#include "lept_value.h"
#include <memory>
#include <string>
#include <string_view>

class lept_context {
public:
    static constexpr size_t GetLengthOfBaseJasonStr(std::string_view sv)
    {
        return sv.length();
    }

    lept_context(const std::string_view& json);
    ~lept_context();

    void parse_whithspace();
    lept_result parse_value(lept_value& value);
    lept_result parse_null(lept_value& value);
    std::string_view getJson() const { return json; }

private:
    std::string_view json;
};

#endif


#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H
#pragma once
#include "lept_enum.h"
#include <cassert>
#include <string>
#include <variant>

namespace LxJson {
using JsonVal = std::variant<std::string, double, bool>;
class lept_value {
public:
    lept_value();
    ~lept_value();
    lept_result parse(const std::string& json);

    inline lept_type getType() const { return type; }
    inline void setType(const lept_type& type_) { type = type_; }

    void setNum(double n_) noexcept
    {
        v = n_;
    }

    void setString(const std::string& string_) { v = string_; }

    void setNull() { type = lept_type::LEPT_NULL; }

    void setBoolean(bool b) { v = b; }

    template <lept_type TypeT>
    auto getValue()
    {
        assert(TypeT == type);
        if constexpr (TypeT == lept_type::LEPT_NULL) {
            return nullptr;
        } else if constexpr (TypeT == lept_type::LEPT_BOOLEAN) {
            return std::get<bool>(v);
        } else if constexpr (TypeT == lept_type::LEPT_NUMBER) {
            return std::get<double>(v);
        } else if constexpr (TypeT == lept_type::LEPT_STRING) {
            return std::get<std::string>(v);
        }
    }

private:
    JsonVal v;
    lept_type type;
};
}

#endif
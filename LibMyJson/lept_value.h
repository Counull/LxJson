

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

#include <exception>
#include <iostream>
#include <ostream>
#pragma once
#include "lept_enum.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <xtr1common>
namespace LxJson {
using JsonVal = std::variant<std::string, double, bool, nullptr_t>;
class lept_value {
public:
    lept_value();
    ~lept_value();
    lept_result parse(const std::string& json);

    inline lept_type getType() const { return type; }

    template <typename T>
    void setValue(T&& val)
    {

        v = std::forward<T>(val);
        type = lept_type::LEPT_NULL;
        if constexpr (std::is_same_v<typename std::decay_t<T>, nullptr_t>) {
            type = lept_type::LEPT_NULL;
        }
        if constexpr (std::_Is_any_of_v<typename std::decay_t<T>, std::string, char const*, char*, char[]>) {

            type = lept_type::LEPT_STRING;

        } else if constexpr (std::is_same_v<typename std::decay_t<T>, bool>) {
            type = lept_type::LEPT_BOOLEAN;

        } else if constexpr (std::is_arithmetic_v<typename std::decay_t<T>>) {
            type = lept_type::LEPT_NUMBER;
        }
    }

    template <lept_type Type>
    auto getValue()
    {
        assert(Type == type);
        if constexpr (Type == lept_type::LEPT_NULL) {
            return nullptr;
        } else if constexpr (Type == lept_type::LEPT_BOOLEAN) {
            return std::get<bool>(v);
        } else if constexpr (Type == lept_type::LEPT_NUMBER) {
            return std::get<double>(v);
        } else if constexpr (Type == lept_type::LEPT_STRING) {
            return std::get<std::string>(v);
        }
    }
    template <typename Type>
    auto getValue()
    {
        return std::get<Type>(v);
    }

private:
    JsonVal v;
    lept_type type;
};
}

#endif
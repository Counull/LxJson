

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

#include <exception>
#include <iostream>
#include <ostream>
#include <tuple>
#include <vector>
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
class lept_value;
using JsonArray = std::vector<lept_value>;
using JsonVal = std::variant<std::string, JsonArray, double, bool, nullptr_t>;

class lept_value {
public:
    lept_value();
    lept_value(lept_value&& val) = default;
    lept_value(const lept_value& val) = default;
    lept_value& operator=(const lept_value& val) = default;
    lept_value& operator=(lept_value&& val) = default;
    template <typename T>
    lept_value(T&& val);

    ~lept_value();
    lept_result parse(const std::string& json);

    inline lept_type getType() const { return type; }

    /// 很显然这么写容易因为疏忽造成运行时错误
    template <typename T>
    void setValue(T&& val)
        requires(std::_Is_any_of_v<typename std::decay_t<T>,
                     nullptr_t, std::string, JsonArray, char const*, char*, char[], bool>
            || std::is_arithmetic_v<typename std::decay_t<T> >)
    {

        v = std::forward<T>(val);
        type = lept_type::LEPT_NULL;
        if constexpr (std::is_same_v<typename std::decay_t<T>, nullptr_t>) {
            type = lept_type::LEPT_NULL;
        } else if constexpr (std::_Is_any_of_v<typename std::decay_t<T>, std::string, char const*, char*, char[]>) {
            type = lept_type::LEPT_STRING;
        } else if constexpr (std::is_same_v<typename std::decay_t<T>, bool>) {
            type = lept_type::LEPT_BOOLEAN;
        } else if constexpr (std::is_arithmetic_v<typename std::decay_t<T>>) {
            type = lept_type::LEPT_NUMBER;
        } else if constexpr (std::is_same_v<typename std::decay_t<T>, JsonArray>) {
            type = lept_type::LEPT_ARRAY;
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
        } else if constexpr (Type == lept_type::LEPT_ARRAY) {
            return std::get<JsonArray>(v);
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

template <typename T>
inline lept_value::lept_value(T&& val)
{
    this->setValue(val);
}

}

#endif
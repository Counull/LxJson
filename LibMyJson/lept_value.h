

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

#include <unordered_map>
#pragma once
#include "lept_enum.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include <xtr1common>
namespace LxJson {
class lept_value;
using JsonArray = std::vector<lept_value>;
using JsonMap = std::unordered_map<std::string, lept_value>;
using JsonVal = std::variant<std::string, JsonArray, JsonMap, double, bool, nullptr_t>;

template <typename T>
concept ValidJsonType = std::_Is_any_of_v<typename std::decay_t<T>,
                            bool, nullptr_t, JsonArray, JsonMap>
    || std::is_convertible_v<typename std::decay_t<T>, std::string>
    || std::is_convertible_v<typename std::decay_t<T>, double>;

bool operator==(const lept_value& lhs, const lept_value& rhs);

class lept_value {
public:
    lept_value();
    lept_value(lept_value&& val) = default;
    lept_value(const lept_value& val) = default;
    lept_value& operator=(const lept_value& val) = default;
    lept_value& operator=(lept_value&& val) = default;

    ~lept_value();

    template <ValidJsonType T>
    lept_value(T&& val)
    {
        setValue(std::forward<T>(val));
    }

    lept_result parse(const std::string& json);
    inline lept_type getType() const { return type; }

    /// 很显然这么写容易因为疏忽造成运行时错误
    template <ValidJsonType T>
    void setValue(T&& val)
    {
        type = lept_type::LEPT_NULL;
        if constexpr (std::is_same_v<typename std::decay_t<T>, nullptr_t>) {
            type = lept_type::LEPT_NULL;
            v = val;
        } else if constexpr (std::is_same_v<typename std::decay_t<T>, bool>) {
            type = lept_type::LEPT_BOOLEAN;
            v = val;
        } else if constexpr (std::is_convertible_v<typename std::decay_t<T>, double>) {
            type = lept_type::LEPT_NUMBER;
            v = static_cast<double>(val);
        } else if constexpr (std::is_convertible_v<std::decay_t<T>, std::string>) {
            type = lept_type::LEPT_STRING;
            v = std::forward<T>(val);
        } else if constexpr (std::is_same_v<typename std::decay_t<T>, JsonArray>) {
            type = lept_type::LEPT_ARRAY;
            v = std::forward<T>(val);
        } else if constexpr (std::is_same_v<typename std::decay_t<T>, JsonMap>) {
            type = lept_type::LEPT_OBJECT;
            v = std::forward<T>(val);
        }
    }

    template <lept_type Type>
    auto getValue() const
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
        } else if constexpr (Type == lept_type::LEPT_OBJECT) {
            return std::get<JsonMap>(v);
        }
    }
    template <ValidJsonType T>
    auto getValue() const
    {
        return std::get<T>(v);
    }

private:
    JsonVal v = nullptr;
    lept_type type = lept_type::LEPT_NULL;
};
}

#endif
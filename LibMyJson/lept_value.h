#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H
#pragma once

#include <exception>

#include "lept_enum.h"
#include <string>
namespace LxJson {
class lept_value {
public:
    lept_value();
    ~lept_value();
    lept_result parse(const std::string& json);

    inline lept_type getType() const { return type; }
    inline void setType(const lept_type& type_) { type = type_; }

    double getNum() const
    {
        if (type != lept_type::LEPT_NUMBER) {
            throw new std::exception("Val invalid");
        }
        return n;
    }
    void setNum(double n_) noexcept { n = n_; }

private:
    lept_type type;
    double n;
};
}
#endif
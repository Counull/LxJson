#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

#pragma once
#include "lept_enum.h"
#include <string>
class lept_value {
public:
    lept_value();
    ~lept_value();
    lept_result parse(const std::string& json);

   inline lept_type getType() const { return type; }
   inline void setType(const lept_type& type_) { type = type_; }

private:
    lept_type type;
};

#endif
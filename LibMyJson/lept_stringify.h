#ifndef LEPT_STRINGIFY_H
#define LEPT_STRINGIFY_H
#include <memory>
#include <optional>
#pragma once

#include "lept_enum.h"
#include "lept_value.h"
#include <string>
namespace LxJson {
class lept_stringify {
public:
    lept_stringify();
    ~lept_stringify();
    std::string json;

    lept_result stringify(lept_value& value);

private:
    lept_result stringify_value(const lept_value& value);
    std::string stringify_string(const std::string& val);
};
}
#endif
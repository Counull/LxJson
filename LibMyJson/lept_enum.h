#ifndef LEPT_ENUM_H
#define LEPT_ENUM_H

#pragma once
namespace LxJson {

enum class lept_type {
    LEPT_BOOLEAN,
    LEPT_NULL,
    LEPT_NUMBER,
    LEPT_STRING,
    LEPT_ARRAY,
    LEPT_OBJECT
};

enum class lept_result {
    LEPT_PARSE_OK,
    LEPT_STRINGIFY_OK,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_NUMBER_TOO_BIG,
    LEPT_PARSE_MISS_QUOTATION_MARK,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_INVALID_STRING_ESCAPE,
    LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
    LEPT_PARSE_MISS_KEY,
    LEPT_PARSE_MISS_COLON,
    LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET
};
}
#endif
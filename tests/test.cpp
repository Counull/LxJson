#include "lept_enum.h"
#include "lept_value.h"
#include <gtest/gtest.h>


TEST(ParseNull, Parse)
{

    lept_value v;
    v.setType(lept_type::LEPT_TRUE);
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("null"));
    ASSERT_EQ(lept_type::LEPT_NULL, v.getType());
}
#include "lept_context.h"
#include "lept_enum.h"
#include "lept_value.h"
#include <gtest/gtest.h>
#include <vcruntime.h>

TEST(Parse, ParseNull)
{
    lept_value v;
    v.setType(lept_type::LEPT_TRUE);
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("null"));
    ASSERT_EQ(lept_type::LEPT_NULL, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(" \t \n \r \r\n  null"));
    ASSERT_EQ(lept_type::LEPT_NULL, v.getType());
    ASSERT_NE(lept_result::LEPT_PARSE_OK, v.parse("\t   \n\r qweqwnull"));
    ASSERT_EQ(lept_type::LEPT_NULL, v.getType());
    ASSERT_NE(lept_result::LEPT_PARSE_OK, v.parse("\t   \n\r nu\rll"));
}

TEST(Parse, ParseEmypt)
{
    lept_context c("     aa");
    c.parse_whithspace();
    ASSERT_EQ("aa", c.getJson());
}

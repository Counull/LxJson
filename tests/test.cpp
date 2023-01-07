#include "lept_context.h"
#include "lept_enum.h"
#include "lept_value.h"
#include <gtest/gtest.h>
#include <vcruntime.h>
using namespace LxJson;

TEST(Parse, ParseNull)
{
    lept_value v;
    v.setType(lept_type::LEPT_TRUE);
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("null"));
    ASSERT_EQ(lept_type::LEPT_NULL, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(" \t \n \r \r\n  null"));
    ASSERT_EQ(lept_type::LEPT_NULL, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r qweqwnull"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r nu\rll"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r nu\0ll"));
    ASSERT_EQ(lept_result::LEPT_PARSE_ROOT_NOT_SINGULAR, v.parse(" \t \n \r \r\n  null \t\rasd"));
}

TEST(Parse, ParseTrue)
{
    lept_value v;
    v.setType(lept_type::LEPT_TRUE);
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("true"));
    ASSERT_EQ(lept_type::LEPT_TRUE, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(" \t \n \r \r\n  true"));
    ASSERT_EQ(lept_type::LEPT_TRUE, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r ture"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r tu re"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r tu\0ll"));
    ASSERT_EQ(lept_result::LEPT_PARSE_ROOT_NOT_SINGULAR, v.parse(" \t \n \r \r\n  true \t\rasd"));
}

TEST(Parse, ParseFalse)
{
    lept_value v;
    v.setType(lept_type::LEPT_TRUE);
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("false"));
    ASSERT_EQ(lept_type::LEPT_FALSE, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(" \t \n \r \r\n  false"));
    ASSERT_EQ(lept_type::LEPT_FALSE, v.getType());
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r fa lse"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r fa\0lse"));
    ASSERT_EQ(lept_result::LEPT_PARSE_ROOT_NOT_SINGULAR, v.parse(" \t \n \r \r\n  false \t\rasd"));
}

void TestNumber(double except, const std::string& json)
{

    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(json));
    ASSERT_EQ(lept_type::LEPT_NUMBER, v.getType());
    ASSERT_DOUBLE_EQ(except, v.getNum());
}

TEST(Parse, ParseNum)
{
    lept_value v;

    TestNumber(0.0, "  0");
    TestNumber(0.0, "-0");
    TestNumber(0.0, "-0.0");

    TestNumber(1.0, "1");
    TestNumber(-1.0, "-1");
    TestNumber(1.5234, "1.5234");
    TestNumber(-1.5234, "-1.5234");

    TestNumber(1E10, "1E10");
    TestNumber(-1E10, "-1E10");
    TestNumber(1e10, "1e10");
    TestNumber(-1e10, "-1e10");

    TestNumber(1E+10, "1E+10");
    TestNumber(-1E+10, "-1E+10");
    TestNumber(1E-10, "1E-10");
    TestNumber(-1E-10, "-1E-10");

    TestNumber(1E+10, "1E+10");
    TestNumber(-1E+10, "-1E+10");
    TestNumber(1E-10, "1E-10");
    TestNumber(-1E-10, "-1E-10");

    TestNumber(1.234E+10, "1.234E+10");
    TestNumber(1.234E-10, "1.234E-10");
}

TEST(Parse, ParseException)
{
    lept_value v;
    v.setType(lept_type::LEPT_TRUE);
    ASSERT_EQ(lept_result::LEPT_PARSE_EXPECT_VALUE, v.parse("          \r \0 "));

    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("+0"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("+1"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse(".123"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("1."));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("INF"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("inf"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("NAN"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("nan"));
    ASSERT_EQ(lept_result::LEPT_PARSE_NUMBER_TOO_BIG, v.parse("-1.7976931348623157e+30800"));
}

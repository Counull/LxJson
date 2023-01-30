#include "lept_enum.h"
#include "lept_value.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utility>
using namespace std::string_literals;
using namespace LxJson;

TEST(Parse, ParseNull)
{
    lept_value v;

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

    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("true"));
    ASSERT_EQ(true, v.getValue<lept_type::LEPT_BOOLEAN>());
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(" \t \n \r \r\n  true"));
    ASSERT_EQ(true, v.getValue<lept_type::LEPT_BOOLEAN>());
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r ture"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r tu re"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r tu\0ll"));
    ASSERT_EQ(lept_result::LEPT_PARSE_ROOT_NOT_SINGULAR, v.parse(" \t \n \r \r\n  true \t\rasd"));
}

TEST(Parse, ParseFalse)
{
    lept_value v;

    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse("false"));
    ASSERT_EQ(false, v.getValue<lept_type::LEPT_BOOLEAN>());
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(" \t \n \r \r\n  false"));
    ASSERT_EQ(false, v.getValue<lept_type::LEPT_BOOLEAN>());
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r fa lse"));
    ASSERT_EQ(lept_result::LEPT_PARSE_INVALID_VALUE, v.parse("\t   \n\r fa\0lse"));
    ASSERT_EQ(lept_result::LEPT_PARSE_ROOT_NOT_SINGULAR, v.parse(" \t \n \r \r\n  false \t\rasd"));
}

void TestNumber(double except, const std::string& json)
{

    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(json));
    ASSERT_EQ(lept_type::LEPT_NUMBER, v.getType());
    ASSERT_DOUBLE_EQ(except, v.getValue<lept_type::LEPT_NUMBER>());
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
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_QUOTATION_MARK, v.parse("\"ab\0c\""));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_QUOTATION_MARK, v.parse("\"abc"));
}
void testStr(const std::string& except, const std::string& json)
{
    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(json));
    ASSERT_EQ(lept_type::LEPT_STRING, v.getType());
    ASSERT_STREQ(except.data(), v.getValue<std::string>().data());
}
TEST(Parse, ParseString)
{
    testStr("abc", "\"abc\"");
    testStr("", "\"\"");
    testStr("abc", "    \"abc\"");
    testStr("  abc  ", "    \"  abc  \"");
    testStr("  a b c  ", "    \"  a b c  \"");
    testStr("  a b\r\n\t\"\b\f/ \\  c  ", "    \"  a b\\r\\n\\t\\\"\\b\\f\\/ \\\\  c  \"");
}

void testArray(const std::string& json, JsonArray& array)
{
    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(json));
    auto type = v.getType();
    ASSERT_EQ(lept_type::LEPT_ARRAY, type);
    auto ret = v.getValue<JsonArray>();
    ASSERT_EQ(ret == array, true);
}

TEST(Parse, ParseArray)
{

    lept_value v1;
    v1.setValue(123.f);
    lept_value v2(v1);
    JsonArray ret;
    ret.emplace_back(nullptr);
    ret.emplace_back(false);
    ret.emplace_back(true);
    ret.emplace_back(123);
    ret.emplace_back("abc");

    testArray("[ null , false , true , 123 , \"abc\" ]", ret);

    ret.clear();
    ret.emplace_back(nullptr);
    ret.emplace_back(false);
    ret.emplace_back(JsonArray());
    ret.emplace_back(123);
    ret.emplace_back("abc");
    testArray("[ null , false , [] , 123 , \"abc\" ]", ret);

    auto inside = JsonArray();
    ret.clear();
    ret.emplace_back(nullptr);
    ret.emplace_back(false);
    inside.emplace_back(true);
    //  inside.emplace_back(false);
    ret.emplace_back(inside);
    ret.emplace_back(123);
    ret.emplace_back("abc");
    testArray("[ null , false , [true] , 123 , \"abc\" ]", ret);
}

TEST(Parse, ParseObjectError)
{
    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COLON, v.parse("{\"a\"}"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COLON, v.parse("{\"a\",\"b\"}"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COLON, v.parse("{\"a\"}"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COLON, v.parse("{\"a\""));

    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{:1,"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{1:1,"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{true:1,"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{false:1,"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{[]:1,"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{{}:1,"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_KEY, v.parse("{\"a\":1,"));

    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET, v.parse("{"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET, v.parse("{\"a\":1"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET, v.parse("{\"a\":"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET, v.parse("{\"a\":1]"));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET, v.parse("{\"a\":1 \"b\""));
    ASSERT_EQ(lept_result::LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET, v.parse("{\"a\":{}"));
}

void testObj(JsonObject map, const std::string& json)
{
    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(json));
    auto type = v.getType();
    ASSERT_EQ(lept_type::LEPT_OBJECT, type);
    auto ret = v.getValue<JsonObject>();
    ASSERT_EQ(ret == map, true);
}

TEST(Parse, ParseObject)
{
    JsonObject map;
    testObj(map, "{ }");

    map.emplace("s", "abc");
    map.emplace("d", 1);
    map.emplace("n", nullptr);
    map.emplace("f", false);
    map.emplace("t", true);
    JsonArray inside;
    inside.emplace_back(true);
    inside.emplace_back(false);
    inside.emplace_back(nullptr);
    inside.emplace_back(1.5);
    inside.emplace_back("abcad");
    map.emplace("a", std::move(inside));
    testObj(map,
        R"({  "s":"abc",
                    "d":1,
                    "n":null,
                    "f":false,
                    "t":true,
                    "a":[true,false,null,1.5,"abcad"]
                })");
}

void testStringify(const std::string& json)
{
    lept_value v;
    ASSERT_EQ(lept_result::LEPT_PARSE_OK, v.parse(json));
    std::string ret;
    ASSERT_EQ(lept_result::LEPT_STRINGIFY_OK, v.stringify(ret));
    ASSERT_STREQ(ret.c_str(), json.c_str());
}

TEST(Stringify, Stringify)
{
    testStringify("null");
    testStringify("true");
    testStringify("false");
    testStringify(R"("123123")");
    testStringify(R"("1231\n23")");
    testStringify("1.565656");
}

TEST(Std, StdVarent)
{
    lept_value v;
    v.setValue(123.5f);
    ASSERT_EQ(v.getType(), lept_type::LEPT_NUMBER);
    v.setValue(123.5);
    ASSERT_EQ(v.getType(), lept_type::LEPT_NUMBER);
    v.setValue("123213"s);
    ASSERT_EQ(v.getType(), lept_type::LEPT_STRING);
    ASSERT_EQ(v.getValue<lept_type::LEPT_STRING>(), "123213"s);
    v.setValue("123213"s);
    ASSERT_EQ(v.getType(), lept_type::LEPT_STRING);
    ASSERT_EQ(v.getValue<lept_type::LEPT_STRING>(), "123213"s);
}

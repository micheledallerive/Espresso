#include <gtest/gtest.h>
#include <utils/string.hpp>

using namespace espresso;

TEST(StringUtils, EqualsCaseInsensitive)
{
    EXPECT_TRUE(equals_case_insensitive("hello", "hello"));
    EXPECT_TRUE(equals_case_insensitive("hello", "HELLO"));
    EXPECT_TRUE(equals_case_insensitive("hello", "HeLLo"));
}

TEST(StringUtils, CompareCaseInsensitive)
{
    EXPECT_EQ(compare_case_insensitive("hello", "hello"), 0);
    EXPECT_EQ(compare_case_insensitive("hello", "HELLO"), 0);
    EXPECT_EQ(compare_case_insensitive("hello", "HeLLo"), 0);
    EXPECT_EQ(compare_case_insensitive("hello", "world"), -1);
    EXPECT_EQ(compare_case_insensitive("world", "hello"), 1);
    EXPECT_EQ(compare_case_insensitive("Hello", "hello world"), -1);
    EXPECT_EQ(compare_case_insensitive("Hello world", "hello"), 1);
}
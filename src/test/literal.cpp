#include <nil/xalt/literal.hpp>

#include <gtest/gtest.h>

using namespace nil::xalt;

TEST(literal, sv)
{
    EXPECT_EQ(literal_sv<"hello">, "hello");
}

TEST(literal, concat)
{
    const auto result = literal_sv<concat<"hello", " ", "world">()>;
    EXPECT_EQ(result, "hello world");
}

TEST(literal, substr)
{
    const auto result = literal_sv<substr<"hello", 2, 2>()>;
    EXPECT_EQ(result, "ll");
}

TEST(literal, find_first)
{
    {
        // found
        const auto result = find_first<"hello", "l">();
        EXPECT_EQ(result, 2);
    }
    {
        // found
        const auto result = find_first<"hello", "e">();
        EXPECT_EQ(result, 1);
    }
    {
        // not found
        const auto result = find_first<"hello", "a">();
        EXPECT_EQ(result, 6);
    }
}

TEST(literal, starts_with)
{
    {
        // found
        const auto result = starts_with<"hello", "he">();
        EXPECT_EQ(result, true);
    }
    {
        // not found
        const auto result = starts_with<"hello", "el">();
        EXPECT_EQ(result, false);
    }
}

TEST(literal, replace_one)
{
    {
        // found one
        const auto result = literal_sv<replace_one<"abcdefg-abcdefg", "cd", "__">()>;
        EXPECT_EQ(result, "ab__efg-abcdefg");
    }
    {
        // found none
        const auto result = literal_sv<replace_one<"abcdefg-abcdefg", "hi", "__">()>;
        EXPECT_EQ(result, "abcdefg-abcdefg");
    }
}

TEST(literal, replace_all)
{
    {
        // found all
        const auto result = literal_sv<replace_all<"abcdefg-abcdefg", "cd", "__">()>;
        EXPECT_EQ(result, "ab__efg-ab__efg");
    }
    {
        // found none
        const auto result = literal_sv<replace_one<"abcdefg-abcdefg", "hi", "__">()>;
        EXPECT_EQ(result, "abcdefg-abcdefg");
    }
}

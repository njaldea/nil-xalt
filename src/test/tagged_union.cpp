// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#include <nil/xalt/tagged_union.hpp>

#include <gtest/gtest.h>

using namespace nil::xalt;

namespace
{
    struct A final
    {
        int value = 0;
    };

    struct B final
    {
    };

    struct C final
    {
        bool flag = false;
        int payload = 0;
    };

    using union_t = tagged_union<A, B, C>;
}

TEST(tagged_union, default_constructs_empty)
{
    const union_t value;

    EXPECT_TRUE(value.is_empty());
    EXPECT_FALSE(value.holds<A>());
    EXPECT_FALSE(value.holds<B>());
    EXPECT_FALSE(value.holds<C>());
}

TEST(tagged_union, converting_constructor_holds_given_alternative)
{
    const union_t value{C{.flag = true, .payload = 42}};

    EXPECT_FALSE(value.is_empty());
    EXPECT_TRUE(value.holds<C>());
    EXPECT_FALSE(value.holds<A>());
    EXPECT_FALSE(value.holds<B>());
    EXPECT_EQ(value.get<C>().payload, 42);
}

TEST(tagged_union, copy_preserves_active_alternative)
{
    const union_t original{C{.flag = true, .payload = 7}};

    const union_t copy = original; // NOLINT(performance-unnecessary-copy-initialization)

    EXPECT_TRUE(copy.holds<C>());
    EXPECT_EQ(copy.get<C>().payload, 7);
}

TEST(tagged_union, assignment_switches_active_alternative)
{
    union_t value{A{.value = 1}};

    value = union_t{C{.flag = false, .payload = 9}};

    EXPECT_TRUE(value.holds<C>());
    EXPECT_FALSE(value.holds<A>());
    EXPECT_EQ(value.get<C>().payload, 9);
}

TEST(tagged_union, visit_invokes_handler_for_active_alternative)
{
    union_t value{C{.flag = true, .payload = 5}};

    const auto result = value.visit(
        [](auto& v) -> int
        {
            using T = std::remove_cvref_t<decltype(v)>;
            if constexpr (std::is_same_v<T, C>)
            {
                return v.payload;
            }
            else
            {
                return -1;
            }
        }
    );

    EXPECT_EQ(result, 5);
}

TEST(tagged_union, visit_supports_void_returning_handler)
{
    union_t value{A{.value = 3}};
    auto observed = 0;

    value.visit(
        [&]<typename T>(T& v)
        {
            if constexpr (std::is_same_v<T, A>)
            {
                observed = v.value;
            }
        }
    );

    EXPECT_EQ(observed, 3);
}

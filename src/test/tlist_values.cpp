#include <nil/xalt/tlist_values.hpp>

#include <gtest/gtest.h>
#include <type_traits>

using namespace nil::xalt;

struct is_same
{
    constexpr bool operator()(auto l, auto r)
    {
        return std::is_same_v<decltype(l), decltype(r)> && l == r;
    }
};

struct add
{
    constexpr auto operator()(auto l, auto r)
    {
        return l + r;
    }
};

TEST(tlist, values)
{
    using type = tlist_values<1, 5, 8>;
    {
        static_assert(type::size == 3);
    }
    {
        using casted_type = std::index_sequence<1, 5, 8>;
        using sut_type = type::cast<std::index_sequence>;
        static_assert(std::is_same_v<sut_type, casted_type>);
    }
    {
        using applied_type = tlist_values<11, 15, 18>;
        using sut_type = type::apply<add, 10>;
        static_assert(std::is_same_v<sut_type, applied_type>);
    }
    {
        static_assert(type::at<0> == 1);
        static_assert(type::at<1> == 5);
        static_assert(type::at<2> == 8);
    }
    {
        static_assert(!type::any_of<is_same, 2>);
        static_assert(type::any_of<is_same, 5>);
    }
    {
        using filtered_1 = type::remove_if<is_same, 3>;
        static_assert(std::is_same_v<filtered_1, tlist_values<1, 5, 8>>);

        using filtered_2 = type::remove_if<is_same, 5>;
        static_assert(std::is_same_v<filtered_2, tlist_values<1, 8>>);
    }
}

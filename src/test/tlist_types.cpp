#include <nil/xalt/tlist_types.hpp>

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

TEST(tlist, types)
{
    using type = tlist_types<int, bool>;
    {
        static_assert(type::size == 2);
    }
    {
        using casted_type = std::tuple<int, bool>;
        using sut_type = type::cast<std::tuple>;
        static_assert(std::is_same_v<sut_type, casted_type>);
    }
    {
        using applied_type = tlist_types<std::tuple<int>, std::tuple<bool>>;
        using sut_type = type::apply<std::tuple>;
        static_assert(std::is_same_v<sut_type, applied_type>);
    }
    {
        static_assert(std::is_same_v<type::at<0>, int>);
        static_assert(std::is_same_v<type::at<1>, bool>);
    }
    {
        static_assert(!type::any_of<std::is_same, float>);
        static_assert(type::any_of<std::is_same, bool>);
    }
    {
        using filtered_1 = type::remove_if<std::is_same, float>;
        static_assert(std::is_same_v<filtered_1, tlist_types<int, bool>>);

        using filtered_2 = type::remove_if<std::is_same, bool>;
        static_assert(std::is_same_v<filtered_2, tlist_types<int>>);
    }
}

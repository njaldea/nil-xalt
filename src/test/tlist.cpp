#include <nil/xalt/tlist.hpp>

#include <gtest/gtest.h>
#include <type_traits>

using namespace nil::xalt;

template <auto T, auto V>
struct is_same
{
    static constexpr auto value = std::is_same_v<decltype(T), decltype(V)> && T == V;
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
}

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
        constexpr auto add_ten = [](auto v) { return v + 10; };
        using applied_type = tlist_values<11, 15, 18>;
        using sut_type = type::apply<add_ten>;
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
}

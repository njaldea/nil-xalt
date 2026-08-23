#include <nil/xalt/tlist.hpp>

#include <gtest/gtest.h>
#include <type_traits>

using namespace nil::xalt;

TEST(tlist, types)
{
    using type = tlist<int, bool>;
    {
        static_assert(type::size == 2);
    }
    {
        using casted_type = std::tuple<int, bool>;
        using sut_type = type::cast<std::tuple>;
        static_assert(std::is_same_v<sut_type, casted_type>);
    }
    {
        using applied_type = tlist<std::tuple<int>, std::tuple<bool>>;
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
        static_assert(!type::contains<float>);
        static_assert(type::contains<bool>);
    }
    {
        static_assert(!tlist<>::contains<int>);
    }
    {
        using filtered_1 = nil::xalt::tlist_remove_if_t<type, std::is_same, float>;
        static_assert(std::is_same_v<filtered_1, tlist<int, bool>>);

        using filtered_2 = nil::xalt::tlist_remove_if_t<type, std::is_same, bool>;
        static_assert(std::is_same_v<filtered_2, tlist<int>>);
    }
    {
        using duplicates = tlist<int, bool, int, float, bool, int>;
        using dedupe = tlist_dedupe_t<duplicates>;
        using expected = tlist<int, bool, float>;

        static_assert(std::is_same_v<dedupe, expected>);
    }
}

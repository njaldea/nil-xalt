#pragma once

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace nil::xalt
{
    template <std::size_t N>
    struct literal final
    {
        consteval literal(const char* init_value, std::size_t offset)
        {
            std::copy_n(&init_value[offset], N - 1, &private_value[0]);
            private_value[N - 1] = '\0';
        }

        // NOLINTNEXTLINE(hicpp-explicit-conversions,*-avoid-c-arrays)
        consteval literal(const char (&init_value)[N])
        {
            std::copy_n(&init_value[0], N, &private_value[0]);
        }

        template <std::size_t M, std::size_t... Rest>
        // NOLINTNEXTLINE(hicpp-explicit-conversions)
        consteval literal(literal<M> init_value_l, literal<Rest>... init_value_r)
        {
            auto it = std::copy_n(&init_value_l.private_value[0], M - 1, &private_value[0]);
            (..., (it = std::copy_n(&init_value_r.private_value[0], Rest - 1, it)));
        }

        // NOLINTNEXTLINE(*-avoid-c-arrays)
        char private_value[N] = {};
    };

    template <literal T>
    inline constexpr const auto& literal_v = T.private_value;
    template <literal T>
    inline constexpr const auto literal_sv = std::string_view(T.private_value);

    template <literal... T>
    consteval auto concat() -> literal<(1 + ... + (sizeof(T) - 1))>
    {
        return literal<(1 + ... + (sizeof(T) - 1))>(T...);
    }

    template <literal N, std::size_t offset, std::size_t size = sizeof(N) - offset>
    consteval auto substr() -> literal<size + 1>
    {
        static_assert(offset + size <= sizeof(N));
        return literal<size + 1>(&N.private_value[0], offset);
    }

    template <literal from, literal to_find>
    consteval auto find_first() -> std::size_t
    {
        constexpr auto i = literal_sv<from>.find_first_of(literal_sv<to_find>);
        if constexpr (i == std::string_view::npos)
        {
            return sizeof(from);
        }
        else
        {
            return i;
        }
    }

    template <literal from, literal to_find>
    consteval auto starts_with() -> bool
    {
        return find_first<substr<from, 0, sizeof(to_find)>(), to_find>() == 0;
    }

    template <literal base, literal from, literal to>
    consteval auto replace_one()
    {
        if constexpr (constexpr auto index1 = find_first<base, from>(); index1 != sizeof(base))
        {
            constexpr auto index2 = index1 + sizeof(from) - 1;
            constexpr auto remaining_size = sizeof(base) - index2;
            constexpr auto section1 = substr<base, 0, index1>();
            constexpr auto section2 = substr<base, index2, remaining_size>();
            return concat<section1, to, section2>();
        }
        else
        {
            return base;
        }
    }

    template <literal base, literal from, literal to>
    consteval auto replace_all()
    {
        if constexpr (constexpr auto index1 = find_first<base, from>(); index1 != sizeof(base))
        {
            constexpr auto index2 = index1 + sizeof(from) - 1;
            constexpr auto remaining_size = sizeof(base) - index2;
            constexpr auto section1 = substr<base, 0, index1>();
            constexpr auto section2 = substr<base, index2, remaining_size>();
            return concat<section1, to, replace_all<section2, from, to>()>();
        }
        else
        {
            return base;
        }
    }
}

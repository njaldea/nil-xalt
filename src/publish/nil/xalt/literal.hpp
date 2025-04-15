#pragma once

#include "errors.hpp"

#include <cstddef>

namespace nil::xalt
{
    template <std::size_t N>
    struct literal final
    {
    public:
        // NOLINTNEXTLINE
        consteval literal(const char* init_value, std::size_t offset)
        {
            copy_n(&init_value[offset], N - 1, &private_value[0]);
            private_value[N - 1] = '\0';
        }

        // NOLINTNEXTLINE
        consteval literal(const char (&init_value)[N])
        {
            copy_n(&init_value[0], N, &private_value[0]);
        }

        template <std::size_t M, std::size_t... Rest>
        // NOLINTNEXTLINE
        consteval literal(literal<M> init_value_l, literal<Rest>... init_value_r)
        {
            auto it = copy_n(&init_value_l.private_value[0], M - 1, &private_value[0]);
            (..., (it = copy_n(&init_value_r.private_value[0], Rest - 1, it)));
        }

        // NOLINTNEXTLINE
        std::size_t private_size = N;

        // NOLINTNEXTLINE
        char private_value[N] = {};

    private:
        consteval char* copy_n(const char* from, std::size_t n, char* to)
        {
            for (auto i = 0U; i < n; ++i)
            {
                to[i] = from[i];
            }
            return &to[n];
        }
    };

    template <literal T>
    struct literal_type final
    {
        static constexpr auto value = T.private_value;
    };

    template <literal T>
    static constexpr auto literal_v = literal_type<T>::value;

    template <literal... T>
    consteval auto concat() -> literal<((1 - sizeof...(T)) + ... + T.private_size)>
    {
        return literal<((1 - sizeof...(T)) + ... + T.private_size)>(T...);
    }

    template <literal N, std::size_t offset, std::size_t size>
    consteval auto substr() -> literal<size + 1>
    {
        return literal<size + 1>(&N.private_value[0], offset);
    }

    template <literal from, literal to_find, std::size_t offset = 0>
    consteval auto find_match() -> std::size_t
    {
        if (from.private_size < to_find.private_size)
        {
            return from.private_size;
        }
        constexpr auto N_no_null = from.private_size - 1;
        constexpr auto M_no_null = to_find.private_size - 1;
        for (std::size_t i = offset; i < (N_no_null - M_no_null); ++i)
        {
            for (std::size_t j = 0; j < M_no_null; ++j)
            {
                if (from.private_value[i + j] != to_find.private_value[j])
                {
                    break;
                }
                if (j + 1 == M_no_null)
                {
                    return i;
                }
            }
        }
        return from.private_size;
    }

    template <literal base, literal from, literal to>
    consteval auto replace_one()
    {
        constexpr auto index1 = find_match<base, from>();
        if constexpr (index1 == base.private_size)
        {
            return base;
        }
        else
        {
            constexpr auto index2 = index1 + from.private_size - 1;
            constexpr auto remaining_size = base.private_size - index2;
            constexpr auto section1 = substr<base, 0, index1>();
            constexpr auto section2 = substr<base, index2, remaining_size>();
            return concat<section1, to, section2>();
        }
    }

    template <literal base, literal from, literal to>
    consteval auto replace_all()
    {
        constexpr auto index1 = find_match<base, from>();
        if constexpr (index1 == base.private_size)
        {
            return base;
        }
        else
        {
            constexpr auto index2 = index1 + from.private_size - 1;
            constexpr auto remaining_size = base.private_size - index2;
            constexpr auto section1 = substr<base, 0, index1>();
            constexpr auto section2 = substr<base, index2, remaining_size>();
            return replace_all<concat<section1, to, section2>(), from, to>();
        }
    }
}

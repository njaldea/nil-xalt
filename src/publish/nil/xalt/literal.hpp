#pragma once

#include <cstddef>
#include <string_view>

namespace nil::xalt
{
    template <std::size_t N>
    struct literal final
    {
    public:
        consteval literal(const char* init_value, std::size_t offset)
        {
            copy_n(&init_value[offset], N - 1, &private_value[0]);
            private_value[N - 1] = '\0';
        }

        // NOLINTNEXTLINE(hicpp-explicit-conversions,*-avoid-c-arrays)
        consteval literal(const char (&init_value)[N])
        {
            copy_n(&init_value[0], N, &private_value[0]);
        }

        template <std::size_t M, std::size_t... Rest>
        // NOLINTNEXTLINE(hicpp-explicit-conversions)
        consteval literal(literal<M> init_value_l, literal<Rest>... init_value_r)
        {
            auto it = copy_n(&init_value_l.private_value[0], M - 1, &private_value[0]);
            (..., (it = copy_n(&init_value_r.private_value[0], Rest - 1, it)));
        }

        // NOLINTNEXTLINE(*-avoid-c-arrays)
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
        static constexpr const auto& value = T.private_value;
        static constexpr const auto value_sv = std::string_view(T.private_value);
    };

    template <literal T>
    static constexpr const auto& literal_v = literal_type<T>::value;
    template <literal T>
    static constexpr const auto& literal_sv = literal_type<T>::value_sv;

    template <literal... T>
    consteval auto concat() -> literal<(1 + ... + (sizeof(T) - 1))>
    {
        return literal<(1 + ... + (sizeof(T) - 1))>(T...);
    }

    template <literal N, std::size_t offset, std::size_t size>
    consteval auto substr() -> literal<size + 1>
    {
        static_assert(offset + size <= sizeof(N));
        return literal<size + 1>(&N.private_value[0], offset);
    }

    template <literal from, literal to_find, std::size_t offset = 0, std::size_t end = sizeof(from)>
    consteval auto find_match() -> std::size_t
    {
        static_assert(offset < sizeof(from));
        if (sizeof(to_find) < sizeof(from))
        {
            constexpr auto N_no_null = end - 1;
            constexpr auto M_no_null = sizeof(to_find) - 1;
            for (std::size_t i = offset; i < end && i < (N_no_null - M_no_null); ++i)
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
        }
        return end;
    }

    template <literal from, literal to_find>
    consteval auto starts_with() -> bool
    {
        return find_match<from, to_find, 0, 1>() == 0;
    }

    template <literal base, literal from, literal to>
    consteval auto replace_one()
    {
        constexpr auto index1 = find_match<base, from>();
        if constexpr (index1 == sizeof(base))
        {
            return base;
        }
        else
        {
            constexpr auto index2 = index1 + sizeof(from) - 1;
            constexpr auto remaining_size = sizeof(base) - index2;
            constexpr auto section1 = substr<base, 0, index1>();
            constexpr auto section2 = substr<base, index2, remaining_size>();
            return concat<section1, to, section2>();
        }
    }

    template <literal base, literal from, literal to>
    consteval auto replace_all()
    {
        constexpr auto index1 = find_match<base, from>();
        if constexpr (index1 == sizeof(base))
        {
            return base;
        }
        else
        {
            constexpr auto index2 = index1 + sizeof(from) - 1;
            constexpr auto remaining_size = sizeof(base) - index2;
            constexpr auto section1 = substr<base, 0, index1>();
            constexpr auto section2 = substr<base, index2, remaining_size>();
            return replace_all<concat<section1, to, section2>(), from, to>();
        }
    }
}

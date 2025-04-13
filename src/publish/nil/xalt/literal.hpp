#pragma once

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

        consteval std::size_t get_size() const
        {
            return N;
        }

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

    template <std::size_t... N>
    consteval auto concat(const literal<N>&... l)
    {
        return literal<((1 - sizeof...(N)) + ... + N)>(l...);
    }

    template <std::size_t... N>
    // NOLINTNEXTLINE
    consteval auto concat(const char (&... l)[N])
    {
        return concat(literal<N>(l)...);
    }

    template <std::size_t offset, std::size_t size, std::size_t N>
    consteval auto substr(const literal<N>& l)
    {
        return literal<size + 1>(&l.private_value[0], offset);
    }

    template <std::size_t N, std::size_t M>
    consteval auto find_match(const literal<N>& from, const literal<M>& to_find) -> std::size_t
    {
        if (M > N)
        {
            return std::size_t(-1);
        }
        constexpr auto N_no_null = N - 1;
        constexpr auto M_no_null = M - 1;
        for (std::size_t i = 0; i < (N_no_null - M_no_null); ++i)
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
        return std::size_t(-1);
    }
}

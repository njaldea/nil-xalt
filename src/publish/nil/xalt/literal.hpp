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

    template <typename... T>
    struct concat;

    template <literal... T>
    struct concat<literal_type<T>...>
    {
        using type = literal_type<literal<((1 - sizeof...(T)) + ... + T.private_size)>(T...)>;
        static constexpr auto value = type::value;
    };

    template <literal N, std::size_t offset, std::size_t size>
    struct substr final
    {
        using type = literal_type<literal<size + 1>(&N.private_value[0], offset)>;
        static constexpr auto value = type::value;
    };

    template <literal from, literal to_find>
    struct find_match final
    {
    private:
        static consteval auto eval() -> std::size_t
        {
            if (from.private_size < to_find.private_size)
            {
                return std::size_t(-1);
            }
            constexpr auto N_no_null = from.private_size - 1;
            constexpr auto M_no_null = to_find.private_size - 1;
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

    public:
        static constexpr auto value = eval();
    };

    template <literal N, literal M, literal O>
    struct replace
    {
    private:
        using match = find_match<N, M>;

    public:
        using type = concat<
            typename substr<N, 0, match::value>::type,
            literal_type<O>,
            typename substr<
                N,                                             // str
                match::value + M.private_size - 1,             // offset
                N.private_size - M.private_size - match::value // size
                >::type                                        //
            >::type;
    };
}

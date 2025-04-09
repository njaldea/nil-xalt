#pragma once

#include <cstddef>
#include <span>

namespace nil::xalt
{
    template <std::size_t N>
    struct tp_literal final
    {
    public:
        // NOLINTNEXTLINE
        consteval tp_literal(std::span<const char> init_value)
        {
            copy_n(init_value.data(), N - 1, &value[0]);
            value[N - 1] = '\0';
        }

        // NOLINTNEXTLINE
        consteval tp_literal(const char (&init_value)[N])
        {
            copy_n(&init_value[0], N, &value[0]);
        }

        template <std::size_t M, std::size_t... Rest>
        // NOLINTNEXTLINE
        consteval tp_literal(const char (&init_value_l)[M], const char (&... init_value_r)[Rest])
        {
            auto it = copy_n(&init_value_l[0], M - 1, &value[0]);
            (..., (it = copy_n(&init_value_r[0], Rest - 1, it)));
        }

        // NOLINTNEXTLINE
        char value[N] = {};

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

    template <std::size_t N, std::size_t... Rest>
    // NOLINTNEXTLINE
    consteval auto concat(const char (&l)[N], const char (&... rest)[Rest])
    {
        constexpr auto size = (N + ... + Rest) - sizeof...(Rest);
        static_assert(size > 0, "Invalid size for concatenation");
        return tp_literal<size>(l, rest...);
    }

    template <tp_literal T>
    struct literal final
    {
        static constexpr auto value = T.value;
    };

    template <tp_literal T>
    static constexpr auto literal_v = literal<T>::value;
}

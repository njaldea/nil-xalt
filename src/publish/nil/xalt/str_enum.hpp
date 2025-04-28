#pragma once

#include "literal.hpp"
#include "str_name.hpp"
#include "tlist.hpp"

#include <string_view>
#include <utility>

namespace nil::xalt
{
    template <typename T>
    struct enum_traits
    {
    };

    namespace detail
    {
        template <typename T, typename I = std::index_sequence<>, typename O = tlist_values<>>
        struct scan_values;

        template <typename T, std::size_t IA, std::size_t... I, T... O>
        struct scan_values<T, std::index_sequence<IA, I...>, tlist_values<O...>>
            : std::conditional_t<
                  starts_with<str_name_value_v<T(IA)>, concat<str_name_type_v<T>, "::">()>(),
                  scan_values<T, std::index_sequence<I...>, tlist_values<O..., T(IA)>>,
                  scan_values<T, std::index_sequence<>, tlist_values<O...>>>
        {
        };

        template <typename T, T... O>
        struct scan_values<T, std::index_sequence<>, tlist_values<O...>>
        {
            using type = tlist_values<O...>;
        };

        template <std::size_t start, std::size_t step, typename T>
        struct values;

        template <std::size_t start, std::size_t step, std::size_t... I>
        struct values<start, step, std::index_sequence<I...>>
        {
            using type = std::index_sequence<((I * step) + start)...>;
        };

        template <typename T>
        consteval std::size_t get_start()
        {
            if constexpr (requires() { enum_traits<T>::start; })
            {
                return enum_traits<T>::start;
            }
            else
            {
                return 0;
            }
        }

        template <typename T>
        consteval std::size_t get_step()
        {
            if constexpr (requires() { enum_traits<T>::step; })
            {
                return enum_traits<T>::step;
            }
            else
            {
                return 1;
            }
        }

        template <typename T>
        consteval std::size_t get_range()
        {
            if constexpr (requires() { enum_traits<T>::end; })
            {
                return (enum_traits<T>::end - get_start<T>()) / get_step<T>();
            }
            else
            {
                return (25 - get_start<T>()) / get_step<T>();
            }
        }

        template <typename T>
        struct scan
        {
            using type = scan_values<
                T,
                typename values<
                    get_start<T>(),
                    get_step<T>(),
                    std::make_index_sequence<get_range<T>()>>::type>::type;
        };
    }

    template <typename T>
    struct enum_values
    {
        using type = typename detail::scan<T>::type;
    };

    template <typename T>
    std::string_view str_enum(T enum_value)
    {
        return []<T... values>(T value, tlist_values<values...>)
        {
            const char* name = nullptr;
            (((name = values == value ? str_name_value_v<values> : nullptr) != nullptr) || ...);
            return name != nullptr ? name : "-";
        }(enum_value, typename enum_values<T>::type());
    }
}

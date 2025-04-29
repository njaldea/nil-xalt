#pragma once

#include "literal.hpp"
#include "str_name.hpp"
#include "tlist.hpp"

#include <string_view>
#include <utility>

namespace nil::xalt
{
    template <typename T>
    struct str_enum_scan;

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

        template <typename T>
        consteval std::size_t scan_start()
        {
            if constexpr (requires() { str_enum_scan<T>::start; })
            {
                return str_enum_scan<T>::start;
            }
            else
            {
                return 0;
            }
        }

        template <typename T>
        consteval std::size_t scan_step()
        {
            if constexpr (requires() { str_enum_scan<T>::step; })
            {
                return str_enum_scan<T>::step;
            }
            else
            {
                return 1;
            }
        }

        template <typename T>
        consteval std::size_t scan_end()
        {
            if constexpr (requires() { str_enum_scan<T>::end; })
            {
                return str_enum_scan<T>::end;
            }
            else
            {
                return 25;
            }
        }

        template <typename T>
        struct scan final
        {
            template <std::size_t... I>
            static auto make_values(std::index_sequence<I...> /* indices */)
                -> std::index_sequence<((I * scan_step<T>()) + scan_start<T>())...>;

            static constexpr auto range = (scan_end<T>() - scan_start<T>()) / scan_step<T>();
            using values = decltype(make_values(std::make_index_sequence<range>()));
            using type = scan_values<T, values>::type;
        };
    }

    template <typename T>
    struct enum_values final
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

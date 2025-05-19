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

    namespace detail::scan
    {
        template <typename T, typename I = tlist_values<>, typename O = tlist_values<>>
        struct values;

        template <typename T, std::size_t IA, std::size_t... I, T... O>
        struct values<T, tlist_values<IA, I...>, tlist_values<O...>>
            : std::conditional_t<
                  starts_with<str_name_value<T(IA)>(), concat<str_name_type<T>(), "::">()>(),
                  values<T, tlist_values<I...>, tlist_values<O..., T(IA)>>,
                  values<T, tlist_values<>, tlist_values<O...>>>
        {
        };

        template <typename T, T... O>
        struct values<T, tlist_values<>, tlist_values<O...>>
        {
            using type = tlist_values<O...>;
        };

        template <typename T>
        consteval std::size_t start();

        template <typename T>
        consteval std::size_t step()
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
        consteval std::size_t end()
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
        consteval std::size_t range()
        {
            return (end<T>() - start<T>()) / step<T>();
        }

        template <typename T>
        constexpr std::size_t increment(std::size_t i)
        {
            return start<T>() + (i * step<T>());
        }

        template <typename T>
        constexpr std::size_t mask(std::size_t i)
        {
            return 0x1 << (i * step<T>());
        }

        template <typename T>
        consteval auto predicate()
        {
            if constexpr (requires() { str_enum_scan<T>::predicate; })
            {
                return str_enum_scan<T>::predicate;
            }
            else
            {
                return &increment<T>;
            }
        }

        template <typename T>
        consteval std::size_t start()
        {
            if constexpr (requires() { str_enum_scan<T>::start; })
            {
                return str_enum_scan<T>::start;
            }
            else if constexpr (requires() { str_enum_scan<T>::predicate; })
            {
                if constexpr (str_enum_scan<T>::predicate == &mask<T>)
                {
                    return 0b1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }

        template <typename T>
        using list = to_tlist_values_t<std::make_index_sequence<range<T>()>>;

        template <typename T>
        using applied_list = list<T>::template apply<predicate<T>()>;

        template <typename T>
        using enumeration = typename values<T, applied_list<T>>::type;
    }

    template <typename T>
    using enum_values = detail::scan::enumeration<T>;

    template <typename T>
    std::string_view str_enum(T enum_value)
    {
        constexpr auto each = []<T... values>(T value, tlist_values<values...>)
        {
            const char* name = nullptr;
            (void)(((name = values == value ? str_name_value_v<values> : nullptr) != nullptr) || ...
            );
            return name != nullptr ? name : "-";
        };
        return each(enum_value, enum_values<T>());
    }
}

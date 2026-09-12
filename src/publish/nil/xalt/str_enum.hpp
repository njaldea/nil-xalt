// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "str_name.hpp"
#include "tlist.hpp"
#include "typed.hpp"

#include <string_view>
#include <type_traits>

namespace nil::xalt
{
    namespace detail
    {
        template <auto value>
        constexpr auto is_valid_enum = !nil::xalt::str_name_sv<typify<value>>.starts_with('(');

        template <typename T>
        constexpr auto has_zero_enum = is_valid_enum<T(0)>;

        template <typename T>
        constexpr auto has_one_enum = is_valid_enum<T(1)>;
    }

    template <typename T>
    struct str_enum_start;

    template <typename T>
        requires(detail::has_zero_enum<T>)
    struct str_enum_start<T>
    {
        static constexpr T value = T(0);
    };

    template <typename T>
        requires(!detail::has_zero_enum<T> && detail::has_one_enum<T>)
    struct str_enum_start<T>
    {
        static constexpr T value = T(1);
    };

    template <typename T>
    constexpr T str_enum_start_v = str_enum_start<T>::value;

    namespace detail
    {
        template <auto v>
        concept has_single_bit = (v != 0) && !(v & (v - 1));

        template <typename T, typename U>
        constexpr U str_enum_start_raw_v = static_cast<U>(str_enum_start_v<T>);

        template <typename T, auto v>
        concept is_masking //
            = (str_enum_start_raw_v<T, decltype(v)> > decltype(v)(0))
            && has_single_bit<str_enum_start_raw_v<T, decltype(v)>> // first should have only 1 bit
            && has_single_bit<v>; // current should have only 1 bit

        template <typename T, T v>
        constexpr auto next()
        {
            using type = decltype(v);
            using raw_t = std::underlying_type_t<type>;
            constexpr auto raw_value = static_cast<raw_t>(v);
            constexpr auto is_masking_enum = is_masking<type, raw_value>;
            if constexpr (!is_masking_enum && is_valid_enum<type(raw_value + 1)>)
            {
                return type(raw_value + 1);
            }
            else if constexpr (is_masking_enum && is_valid_enum<type(raw_value << 1)>)
            {
                return type(raw_value << 1);
            }
            else
            {
                return str_enum_start_v<type>;
            }
        }

        template <typename T>
        constexpr T enum_start_v = str_enum_start_v<T>;

        template <typename T, typename L, T I, bool done = (enum_start_v<T> == I)>
        struct values;

        template <typename T, T... L, T I>
        struct values<T, tlist<typify<L>...>, I, false> final
        {
            using type = typename values<T, tlist<typify<L>..., typify<T(I)>>, next<T, I>()>::type;
        };

        template <typename T, typename U, T I>
        struct values<T, U, I, true> final
        {
            using type = U;
        };

    }

    template <typename T>
    struct str_enum_values final
    {
        static constexpr auto start = str_enum_start_v<T>;
        static_assert(detail::is_valid_enum<start>);
        using type = typename detail::values< //
            T,
            tlist<typify<start>>,
            detail::next<T, start>()>::type;
    };

    template <typename T>
    using str_enum_values_t = typename str_enum_values<T>::type;

    template <typename T>
    std::string_view str_enum(T enum_value)
    {
        static constexpr auto each = []<T... v>(T value, tlist<typify<v>...>)
        {
            const char* name = nullptr;
            (void)(                                                                  //
                (nullptr != (name = (value == v ? str_name_v<typify<v>> : nullptr))) //
                || ...                                                               //
                || (name = "-")
            );
            return name;
        };
        return each(enum_value, str_enum_values_t<T>());
    }

    template <typename T>
    std::string_view str_short_enum(T enum_value)
    {
        static constexpr auto each = []<T... v>(T value, tlist<typify<v>...>)
        {
            const char* name = nullptr;
            (void)(                                                                        //
                (nullptr != (name = (value == v ? str_short_name_v<typify<v>> : nullptr))) //
                || ...                                                                     //
                || (name = "-")
            );
            return name;
        };
        return each(enum_value, str_enum_values_t<T>());
    }
}

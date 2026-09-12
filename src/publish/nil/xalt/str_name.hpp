// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#pragma once

#if !defined(__clang__) && !defined(__GNUC__) && !defined(_MSC_VER)
#include "errors.hpp"
#endif

#include "literal.hpp"
#include "typed.hpp"

namespace nil::xalt::detail
{
    template <literal U>
    static consteval auto pretty_function()
    {
        constexpr auto pretty_print = literal_sv<U>;

        constexpr auto pos1 = pretty_print.find_first_of('=') + 2;
        constexpr auto pos2 = pretty_print.find_last_of(']');

        return literal<pos2 - pos1 + 1>(pretty_print.data(), pos1);
    }

    template <literal U>
    static consteval auto funcsig()
    {
        constexpr auto pretty_print = literal_sv<U>;

        constexpr auto p = pretty_print.find_first_of('<');
        constexpr auto f_e = pretty_print.find("<enum ", p);
        constexpr auto f_c = pretty_print.find("<class ", p);
        constexpr auto f_s = pretty_print.find("<struct ", p);

        constexpr auto pos1 = p + (f_e == p ? 6 : (f_c == p ? 7 : (f_s == p ? 8 : 1)));
        constexpr auto pos2 = pretty_print.find_last_of('>');

        return literal<pos2 - pos1 + 1>(pretty_print.data(), pos1);
    }

    template <typename T>
    consteval auto str_lit()
    {
#if defined(__clang__) || defined(__GNUC__)
        return detail::pretty_function<__PRETTY_FUNCTION__>();
#elif defined(_MSC_VER)
        return detail::funcsig<__FUNCSIG__>();
#else
        undefined<T>();
        return "not supported";
#endif
    }

    template <auto T>
    consteval auto str_lit()
    {
#if defined(__clang__) || defined(__GNUC__)
        return detail::pretty_function<__PRETTY_FUNCTION__>();
#elif defined(_MSC_VER)
        return detail::funcsig<__FUNCSIG__>();
#else
        undefined<T>();
        return "not supported";
#endif
    }

    template <typename T>
    struct str_name_dispatch
    {
        static consteval auto name()
        {
            return str_lit<T>();
        }
    };

    template <auto T>
    struct str_name_dispatch<typify<T>>
    {
        static consteval auto name()
        {
            return str_lit<T>();
        }
    };

    template <literal T>
    consteval auto str_name_index() -> std::size_t
    {
        constexpr auto name_sv = literal_sv<T>;
        if constexpr (!name_sv.ends_with('>'))
        {
            constexpr auto colon = rfind<T, ":">();
            return colon == sizeof(T) ? 0UL : (colon + 1UL);
        }

        constexpr auto size = name_sv.size();
        auto template_depth = 0;
        for (auto pos = size; pos > 0UL; --pos)
        {
            const auto c = name_sv[pos - 1UL];

            if (c == '>')
            {
                template_depth += 1;
                continue;
            }

            if (c == '<')
            {
                --template_depth;
                continue;
            }

            if (template_depth == 0 && c == ':')
            {
                return pos;
            }
        }
        return 0UL;
    }

    template <literal T>
    consteval auto short_base_name_of()
    {
        constexpr auto sv = literal_sv<T>;
        constexpr auto lt = sv.find('<');
        if constexpr (lt == std::string_view::npos)
        {
            return T;
        }
        else
        {
            return substr<T, 0, lt>();
        }
    }

    template <typename T>
    struct refl_cache
    {
        static constexpr auto name = str_name_dispatch<T>::name();
        static constexpr auto name_sv = nil::xalt::literal_sv<name>;

        static constexpr auto index = str_name_index<name>();

        static constexpr auto short_name = substr<name, index>();
        static constexpr auto short_name_sv = nil::xalt::literal_sv<short_name>;

        static constexpr auto scope_name = substr<name, 0, index - (index > 2UL ? 2UL : 0UL)>();
        static constexpr auto scope_name_sv = nil::xalt::literal_sv<scope_name>;

        static constexpr auto base_name = short_base_name_of<short_name>();
        static constexpr auto base_name_sv = nil::xalt::literal_sv<base_name>;
    };
}

namespace nil::xalt
{
    template <typename T>
    constexpr const auto& str_name()
    {
        return detail::refl_cache<T>::name;
    }

    template <typename T>
    constexpr const auto& str_short_name()
    {
        return detail::refl_cache<T>::short_name;
    }

    template <typename T>
    constexpr const auto& str_scope_name()
    {
        return detail::refl_cache<T>::scope_name;
    }

    template <typename T>
    constexpr const auto& str_short_base_name()
    {
        return detail::refl_cache<T>::base_name;
    }

    template <typename T>
    inline constexpr const auto& str_name_v = literal_v<detail::refl_cache<T>::name>;

    template <typename T>
    inline constexpr const auto& str_name_sv = literal_sv<detail::refl_cache<T>::name>;

    template <typename T>
    inline constexpr const auto& str_short_name_v = literal_v<detail::refl_cache<T>::short_name>;

    template <typename T>
    inline constexpr const auto& str_short_name_sv = detail::refl_cache<T>::short_name_sv;

    template <typename T>
    inline constexpr const auto& str_scope_name_v = literal_v<detail::refl_cache<T>::scope_name>;

    template <typename T>
    inline constexpr const auto& str_scope_name_sv = detail::refl_cache<T>::scope_name_sv;

    template <typename T>
    inline constexpr const auto& str_short_base_name_v
        = nil::xalt::literal_v<detail::refl_cache<T>::base_name>;

    template <typename T>
    inline constexpr const auto& str_short_base_name_sv = detail::refl_cache<T>::base_name_sv;
}

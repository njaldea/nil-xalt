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
        constexpr auto f = pretty_print.find("<enum ", p);

        constexpr auto pos1 = p + (f == p ? 6 : 1);
        constexpr auto pos2 = pretty_print.find_last_of('>');

        return literal<pos2 - pos1 + 1>(pretty_print.data(), pos1);
    }

    template <typename T>
    struct str_name
    {
        static consteval auto name()
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
    };

    template <auto value>
    struct str_name<typify<value>>
    {
        static consteval auto name()
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
    };
}

namespace nil::xalt
{
    template <typename T>
    consteval auto str_name()
    {
        return detail::str_name<T>::name();
    }

    template <typename T>
    inline constexpr const auto& str_name_v = literal_v<str_name<T>()>;

    template <typename T>
    inline constexpr const auto& str_name_sv = literal_sv<str_name<T>()>;
}

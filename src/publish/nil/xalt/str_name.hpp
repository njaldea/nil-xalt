#pragma once

#include "literal.hpp"

namespace nil::xalt
{
    namespace detail
    {
        template <literal U>
        static consteval auto pretty_function()
        {
            using type = literal_type<U>;
            constexpr auto pretty_print = std::string_view(type::value);

            // +2 is to move the starting position to the start of YourType
            constexpr auto pos1 = pretty_print.find_first_of('=') + 2;
            constexpr auto pos2 = pretty_print.find_first_of(']', pos1);

            // +1 is for the null character
            return literal<pos2 - pos1 + 1>(pretty_print.data(), pos1);
        }

        template <literal U>
        static consteval auto funcsig()
        {
            using type = literal_type<U>;
            constexpr auto pretty_print = std::string_view(type::value);

            constexpr auto p = pretty_print.find_first_of('<');
            constexpr auto f = pretty_print.find("<enum ", p);

            constexpr auto pos1 = p + (f == p ? 6 : 1);
            constexpr auto pos2 = pretty_print.find_first_of('>', pos1);
            return literal<pos2 - pos1 + 1>(pretty_print.data(), pos1);
        }
    }

    template <typename T>
    consteval auto str_name_type()
    {
#if defined(__clang__) || defined(__GNUC__)
        return detail::pretty_function<__PRETTY_FUNCTION__>();
#elif defined(_MSC_VER)
        return detail::funcsig<__FUNCSIG__>();
#else
        return "not supported";
#endif
    }

    template <auto T>
    consteval auto str_name_value()
    {
#if defined(__clang__) || defined(__GNUC__)
        return detail::pretty_function<__PRETTY_FUNCTION__>();
#elif defined(_MSC_VER)
        return detail::funcsig<__FUNCSIG__>();
#else
        return "not supported";
#endif
    }

    template <typename T>
    inline constexpr const auto& str_name_type_v = literal_v<str_name_type<T>()>;

    template <typename T>
    inline constexpr const auto& str_name_type_sv = literal_sv<str_name_type<T>()>;

    template <auto T>
    inline constexpr const auto& str_name_value_v = literal_v<str_name_value<T>()>;

    template <auto T>
    inline constexpr const auto& str_name_value_sv = literal_sv<str_name_value<T>()>;
}

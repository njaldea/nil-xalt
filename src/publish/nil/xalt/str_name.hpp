#pragma once

#include "detail/tparam_name.hpp"
#include "literal.hpp"

namespace nil::xalt
{
    template <typename T>
    struct str_name_type final
    {
        static consteval auto name()
        {
            // this is to be replaced when reflection is available
            return detail::tparam_name<__PRETTY_FUNCTION__>();
        }
    };

    template <auto T>
    struct str_name_value final
    {
        static consteval auto name()
        {
            // this is to be replaced when reflection is available
            return detail::tparam_name<__PRETTY_FUNCTION__>();
        }
    };

    template <typename T>
    static constexpr const auto& str_name_type_v = literal_v<str_name_type<T>::name()>;

    template <typename T>
    static constexpr const auto& str_name_type_sv = literal_sv<str_name_type<T>::name()>;

    template <auto T>
    static constexpr const auto& str_name_value_v = literal_v<str_name_value<T>::name()>;

    template <auto T>
    static constexpr const auto& str_name_value_sv = literal_sv<str_name_value<T>::name()>;
}

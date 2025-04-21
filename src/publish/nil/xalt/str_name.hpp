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
            return detail::tparam_name<__PRETTY_FUNCTION__>();
        }

        static constexpr auto value = literal_v<name()>;
    };

    template <auto T>
    struct str_name_value final
    {
        static consteval auto name()
        {
            return detail::tparam_name<__PRETTY_FUNCTION__>();
        }

        static constexpr auto value = literal_v<name()>;
    };

    template <typename T>
    static constexpr auto str_name_type_v = str_name_type<T>::value;

    template <auto T>
    static constexpr auto str_name_value_v = str_name_value<T>::value;
}

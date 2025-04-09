#pragma once

#include "value_name.hpp"

#include <string_view>

namespace nil::xalt
{
    template <typename T, T First, T... Rest>
    std::string_view enum_name(T value)
    {
        if (value == First)
        {
            return value_name_v<First>;
        }
        if constexpr (sizeof...(Rest) > 0)
        {
            return enum_name<T, Rest...>(value);
        }
        else
        {
            return "-";
        }
    }
}

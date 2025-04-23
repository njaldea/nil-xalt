#pragma once

#include "str_name.hpp"

#include <string_view>

namespace nil::xalt
{
    template <auto First, decltype(First)... Rest>
    std::string_view stringify(decltype(First) value)
    {
        // this is to be replaced when reflection is available
        if (value == First)
        {
            return str_name_value_v<First>;
        }
        if constexpr (sizeof...(Rest) > 0)
        {
            return stringify<Rest...>(value);
        }
        else
        {
            return "-";
        }
    }
}

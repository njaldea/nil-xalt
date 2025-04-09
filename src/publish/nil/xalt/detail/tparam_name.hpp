#pragma once

#include "../Literal.hpp"

#include <string_view>

namespace nil::xalt::detail
{
    // expected to be
    // "static consteval auto ValueName<T>::get_type_name() [with T = YourType]"
    template <auto U>
    static consteval auto tparam_name()
    {
        constexpr auto pretty_print = std::string_view(U.value);
        // +2 is to move the starting position to the start of YourType
        constexpr auto pos1 = pretty_print.find_first_of('=') + 2;
        constexpr auto pos2 = pretty_print.find_first_of(']', pos1);
        constexpr auto str = std::span(
            pretty_print.begin() + pos1,
            pretty_print.begin() + pos2 //
        );

        // +1 is for the null character
        return TPLiteral<pos2 - pos1 + 1>(str);
    }
}

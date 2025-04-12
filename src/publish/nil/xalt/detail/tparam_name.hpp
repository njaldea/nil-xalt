#pragma once

#include "../literal.hpp"

#include <string_view>

namespace nil::xalt::detail
{
    // expected to be
    // "static consteval auto ValueName<T>::get_type_name() [with T = YourType]"
    template <tp_literal U>
    static consteval auto tparam_name()
    {
        constexpr auto pretty_print = std::string_view(U.value);
        // +2 is to move the starting position to the start of YourType
        constexpr auto pos1 = pretty_print.find_first_of('=') + 2;
        constexpr auto pos2 = pretty_print.find_first_of(']', pos1);

        // +1 is for the null character
        return tp_literal<pos2 - pos1 + 1>(pretty_print.data(), pos1);
    }
}

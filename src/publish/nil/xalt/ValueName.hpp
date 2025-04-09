#pragma once

#include "detail/tparam_name.hpp"

namespace nil::xalt
{
    template <auto T>
    struct ValueName final
    {
    private:
        static consteval auto name()
        {
            return detail::tparam_name<TPLiteral(__PRETTY_FUNCTION__)>();
        }

    public:
        static constexpr auto value = Literal<name()>::value;
    };
}

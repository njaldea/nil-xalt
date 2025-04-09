#pragma once

#include "detail/tparam_name.hpp"
#include "literal.hpp"

namespace nil::xalt
{
    template <auto T>
    struct value_name final
    {
    private:
        static consteval auto name()
        {
            return detail::tparam_name<tp_literal(__PRETTY_FUNCTION__)>();
        }

    public:
        static constexpr auto value = literal_v<name()>;
    };

    template <auto T>
    static constexpr auto value_name_v = value_name<T>::value;
}

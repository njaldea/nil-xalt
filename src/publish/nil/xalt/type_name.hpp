#pragma once

#include "detail/tparam_name.hpp"
#include "literal.hpp"

namespace nil::xalt
{
    template <typename T>
    struct type_name final
    {
    private:
        static consteval auto name()
        {
            return detail::tparam_name<tp_literal(__PRETTY_FUNCTION__)>();
        }

    public:
        static constexpr auto value = literal_v<name()>;
    };

    template <typename T>
    static constexpr auto type_name_v = type_name<T>::value;
}

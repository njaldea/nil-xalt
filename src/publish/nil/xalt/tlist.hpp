#pragma once

namespace nil::xalt
{
    template <typename... T>
    struct tlist_types final
    {
        static constexpr auto size = sizeof...(T);
    };

    template <auto... T>
    struct tlist_values final
    {
        static constexpr auto size = sizeof...(T);
    };
}

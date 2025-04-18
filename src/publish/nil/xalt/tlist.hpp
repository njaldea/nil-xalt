#pragma once

namespace nil::xalt
{
    template <typename... T>
    struct tlist_types
    {
        static constexpr auto size = sizeof...(T);
    };

    template <auto... T>
    struct tlist_values
    {
        static constexpr auto size = sizeof...(T);
    };
}

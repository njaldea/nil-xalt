#pragma once

namespace nil::xalt
{
    template <typename... T>
    struct tlist_types final
    {
        template <template <typename...> typename U>
        using cast = U<T...>;

        template <template <typename...> typename U>
        using apply = tlist_types<U<T>...>;

        static constexpr auto size = sizeof...(T);
    };

    template <auto... T>
    struct tlist_values final
    {
        template <template <auto...> typename U>
        using cast = U<T...>;

        template <auto P>
        using apply = tlist_values<P(T)...>;

        static constexpr auto size = sizeof...(T);
    };

    template <typename T>
    struct to_tlist_types;

    template <template <typename...> typename T, typename... U>
    struct to_tlist_types<T<U...>>
    {
        using type = tlist_types<U...>;
    };

    template <typename T>
    struct to_tlist_values;

    template <template <auto...> typename T, auto... U>
    struct to_tlist_types<T<U...>>
    {
        using type = tlist_values<U...>;
    };
}

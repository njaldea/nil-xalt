#pragma once

#include <cstddef>

namespace nil::xalt
{
    template <std::size_t I, typename First, typename... T>
    struct tlist_type_at
    {
        using type = typename tlist_type_at<I - 1, T...>::type;
    };

    template <typename First, typename... T>
    struct tlist_type_at<0, First, T...>
    {
        using type = First;
    };

    template <std::size_t I, auto First, auto... T>
    struct tlist_value_at
    {
        using type = typename tlist_value_at<I - 1, T...>::type;
    };

    template <auto First, auto... T>
    struct tlist_value_at<0, First, T...>
    {
        static constexpr auto value = First;
    };

    template <typename... T>
    struct tlist_types final
    {
        template <template <typename...> typename U>
        using cast = U<T...>;

        template <template <typename...> typename U>
        using apply = tlist_types<U<T>...>;

        static constexpr auto size = sizeof...(T);

        template <std::size_t I>
            requires(size > I)
        using at = tlist_type_at<I, T...>::type;
    };

    template <auto... T>
    struct tlist_values final
    {
        template <template <auto...> typename U>
        using cast = U<T...>;

        template <auto P>
        using apply = tlist_values<P(T)...>;

        static constexpr auto size = sizeof...(T);

        template <std::size_t I>
            requires(size > I)
        using at = tlist_value_at<I, T...>::value;
    };

    template <typename T>
    struct to_tlist_types;

    template <template <typename...> typename T, typename... U>
    struct to_tlist_types<T<U...>>
    {
        using type = tlist_types<U...>;
    };

    template <typename T>
    using to_tlist_types_t = typename to_tlist_types<T>::type;

    template <typename T>
    struct to_tlist_values;

    template <template <auto...> typename T, auto... U>
    struct to_tlist_values<T<U...>>
    {
        using type = tlist_values<U...>;
    };

    template <typename T>
    using to_tlist_values_t = typename to_tlist_values<T>::type;
}

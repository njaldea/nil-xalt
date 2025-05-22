#pragma once

#include <cstddef>
#include <utility>

namespace nil::xalt
{
    // types

    template <std::size_t I, typename... T>
    struct tlist_type_at;

    template <std::size_t I, typename First, typename... T>
    struct tlist_type_at<I, First, T...>
    {
        using type = typename tlist_type_at<I - 1, T...>::type;
    };

    template <typename First, typename... T>
    struct tlist_type_at<0, First, T...>
    {
        using type = First;
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
        using at = typename tlist_type_at<I, T...>::type;

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto any_of = (P<T, C...>::value || ... || false);

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto all_of = (P<T, C...>::value && ... && true);
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

    // values

    template <std::size_t I, auto... T>
    struct tlist_value_at;

    template <std::size_t I, auto First, auto... T>
    struct tlist_value_at<I, First, T...>
    {
        static constexpr auto value = tlist_value_at<I - 1, T...>::value;
    };

    template <auto First, auto... T>
    struct tlist_value_at<0, First, T...>
    {
        static constexpr auto value = First;
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
        static constexpr auto at = tlist_value_at<I, T...>::value;

        template <template <auto, auto...> typename P, auto... C>
        static constexpr auto any_of = (P<T, C...>::value || ... || false);

        template <template <auto, auto...> typename P, auto... C>
        static constexpr auto all_of = (P<T, C...>::value && ... && true);
    };

    template <typename T>
    struct to_tlist_values;

    template <template <auto...> typename T, auto... U>
    struct to_tlist_values<T<U...>>
    {
        using type = tlist_values<U...>;
    };

    template <typename T, T... U>
    struct to_tlist_values<std::integer_sequence<T, U...>>
    {
        using type = tlist_values<U...>;
    };

    template <typename T>
    using to_tlist_values_t = typename to_tlist_values<T>::type;
}

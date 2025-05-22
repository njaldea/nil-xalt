#pragma once

#include <cstddef>
#include <utility>

namespace nil::xalt
{
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

    template <typename I, typename O, typename P, auto... T>
    struct tlist_value_remove_if;

    template <auto... T>
    struct tlist_values final
    {
        // This is not standard but might work with gcc/clang
        template <template <auto...> typename U>
        using cast = U<T...>;

        template <typename P, auto... C>
        using apply = tlist_values<P()(T, C...)...>;

        static constexpr auto size = sizeof...(T);

        template <std::size_t I>
            requires(size > I)
        static constexpr auto at = tlist_value_at<I, T...>::value;

        template <typename P, auto... C>
        static constexpr auto any_of = (P()(T, C...) || ... || false);

        template <typename P, auto... C>
        static constexpr auto all_of = (P()(T, C...) && ... && true);

        template <typename P, auto... C>
        using remove_if =
            typename tlist_value_remove_if<tlist_values<T...>, tlist_values<>, P, C...>::type;
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

    template <auto IA, auto... I, auto... O, typename P, auto... T>
    struct tlist_value_remove_if<tlist_values<IA, I...>, tlist_values<O...>, P, T...>
    {
        using type = std::conditional_t<
            P()(IA, T...),
            typename tlist_value_remove_if<tlist_values<I...>, tlist_values<O...>, P, T...>::type,
            typename tlist_value_remove_if<tlist_values<I...>, tlist_values<O..., IA>, P, T...>::
                type>;
    };

    template <auto... O, typename P, auto... T>
    struct tlist_value_remove_if<tlist_values<>, tlist_values<O...>, P, T...>
    {
        using type = tlist_values<O...>;
    };
}

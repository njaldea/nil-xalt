#pragma once

#include <cstddef>
#include <type_traits>

namespace nil::xalt
{
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

    template <typename I, typename O, template <typename, typename...> typename C, typename... T>
    struct tlist_type_remove_if;

    template <typename... T>
    struct tlist_types final
    {
        template <template <typename...> typename U>
        using cast = U<T...>;

        template <template <typename...> typename U, typename... C>
        using apply = tlist_types<U<T, C...>...>;

        static constexpr auto size = sizeof...(T);

        template <std::size_t I>
            requires(size > I)
        using at = typename tlist_type_at<I, T...>::type;

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto any_of = (P<T, C...>::value || ... || false);

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto all_of = (P<T, C...>::value && ... && true);

        template <template <typename, typename...> typename P, typename... C>
        using remove_if =
            typename tlist_type_remove_if<tlist_types<T...>, tlist_types<>, P, C...>::type;
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

    template <
        typename IA,
        typename... I,
        typename... O,
        template <typename, typename...>
        typename C,
        typename... T>
    struct tlist_type_remove_if<tlist_types<IA, I...>, tlist_types<O...>, C, T...>
    {
        using type = std::conditional_t<
            C<IA, T...>::value,
            typename tlist_type_remove_if<tlist_types<I...>, tlist_types<O...>, C, T...>::type,
            typename tlist_type_remove_if<tlist_types<I...>, tlist_types<O..., IA>, C, T...>::type>;
    };

    template <typename... O, template <typename, typename...> typename C, typename... T>
    struct tlist_type_remove_if<tlist_types<>, tlist_types<O...>, C, T...>
    {
        using type = tlist_types<O...>;
    };
}

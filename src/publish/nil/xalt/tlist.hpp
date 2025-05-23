#pragma once

#include "typed.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace nil::xalt
{
    // tlist at to be changed in c++26 with variadic indexing

    template <std::size_t I, typename... T>
    struct tlist_at;

    template <std::size_t I, typename First, typename... T>
    struct tlist_at<I, First, T...>
    {
        static_assert(I < sizeof...(T) + 1, "Index out of bounds");
        using type = typename tlist_at<I - 1, T...>::type;
    };

    template <typename First, typename... T>
    struct tlist_at<0, First, T...>
    {
        using type = First;
    };

    template <typename I, typename O, template <typename, typename...> typename C, typename... T>
    struct tlist_remove_if;

    template <typename... T>
    struct tlist final
    {
        template <template <typename...> typename U>
        using cast = U<T...>;

        template <template <typename...> typename U>
        using cast_t = typename cast<U>::type;

        template <template <typename, typename...> typename U, typename... C>
        using apply = tlist<U<T, C...>...>;

        template <template <typename, typename...> typename U, typename... C>
        using apply_t = tlist<typename U<T, C...>::type...>;

        template <template <typename, typename...> typename P, typename... C>
        using remove_if = typename tlist_remove_if<tlist<T...>, tlist<>, P, C...>::type;

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto any_of = (P<T, C...>::value || ... || false);

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto all_of = (P<T, C...>::value && ... && true);

        static constexpr auto size = sizeof...(T);

        template <std::size_t I>
            requires(size > I)
        using at = typename tlist_at<I, T...>::type;
    };

    template <typename T>
    struct to_tlist;

    template <template <typename...> typename T, typename... U>
    struct to_tlist<T<U...>>
    {
        using type = tlist<U...>;
    };

    template <typename T, T... U>
    struct to_tlist<std::integer_sequence<T, U...>>
    {
        using type = tlist<typify<U>...>;
    };

    template <typename T>
    using to_tlist_t = typename to_tlist<T>::type;

    template <
        typename IA,
        typename... I,
        typename... O,
        template <typename, typename...>
        typename C,
        typename... T>
    struct tlist_remove_if<tlist<IA, I...>, tlist<O...>, C, T...>
    {
        using type = std::conditional_t<
            C<IA, T...>::value,
            typename tlist_remove_if<tlist<I...>, tlist<O...>, C, T...>::type,
            typename tlist_remove_if<tlist<I...>, tlist<O..., IA>, C, T...>::type>;
    };

    template <typename... O, template <typename, typename...> typename C, typename... T>
    struct tlist_remove_if<tlist<>, tlist<O...>, C, T...>
    {
        using type = tlist<O...>;
    };
}

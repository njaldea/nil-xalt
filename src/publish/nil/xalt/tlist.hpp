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
    struct tlist_at<I, First, T...> final
    {
        static_assert(I < sizeof...(T) + 1, "Index out of bounds");
        using type = typename tlist_at<I - 1, T...>::type;
    };

    template <typename First, typename... T>
    struct tlist_at<0, First, T...> final
    {
        using type = First;
    };

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
        static constexpr auto any_of = (P<T, C...>::value || ... || false);

        template <template <typename, typename...> typename P, typename... C>
        static constexpr auto all_of = (P<T, C...>::value && ... && true);

        template <typename U>
        static constexpr auto contains = any_of<std::is_same, U>;

        static constexpr auto size = sizeof...(T);

        template <std::size_t I>
            requires(size > I)
        using at = typename tlist_at<I, T...>::type;
    };

    // to tlist

    template <typename T>
    struct to_tlist;

    template <template <typename...> typename T, typename... U>
    struct to_tlist<T<U...>> final
    {
        using type = tlist<U...>;
    };

    template <typename T, T... U>
    struct to_tlist<std::integer_sequence<T, U...>> final
    {
        using type = tlist<typify<U>...>;
    };

    template <typename T>
    using to_tlist_t = typename to_tlist<T>::type;

    // remove_if

    template <typename I, typename O, template <typename, typename...> typename C, typename... T>
    struct tlist_remove_if_impl;

    template <
        typename IA,
        typename... I,
        typename... O,
        template <typename, typename...>
        typename C,
        typename... T>
    struct tlist_remove_if_impl<tlist<IA, I...>, tlist<O...>, C, T...> final
    {
        using type = std::conditional_t<
            C<IA, T...>::value,
            typename tlist_remove_if_impl<tlist<I...>, tlist<O...>, C, T...>::type,
            typename tlist_remove_if_impl<tlist<I...>, tlist<O..., IA>, C, T...>::type>;
    };

    template <typename... O, template <typename, typename...> typename C, typename... T>
    struct tlist_remove_if_impl<tlist<>, tlist<O...>, C, T...> final
    {
        using type = tlist<O...>;
    };

    template <typename I, template <typename, typename...> typename C, typename... T>
    struct tlist_remove_if final
    {
        using type = typename tlist_remove_if_impl<I, tlist<>, C, T...>::type;
    };

    template <typename I, template <typename, typename...> typename C, typename... T>
    using tlist_remove_if_t = typename tlist_remove_if<I, C, T...>::type;

    // dedupe

    template <typename T>
    struct tlist_dedupe;

    template <typename I, typename O>
    struct tlist_dedupe_impl;

    template <typename IA, typename... I, typename... O>
    struct tlist_dedupe_impl<tlist<IA, I...>, tlist<O...>> final
    {
        using type = std::conditional_t<
            tlist<O...>::template any_of<std::is_same, IA>,
            typename tlist_dedupe_impl<tlist<I...>, tlist<O...>>::type,
            typename tlist_dedupe_impl<tlist<I...>, tlist<O..., IA>>::type>;
    };

    template <typename... O>
    struct tlist_dedupe_impl<tlist<>, tlist<O...>> final
    {
        using type = tlist<O...>;
    };

    template <typename... T>
    struct tlist_dedupe<tlist<T...>> final
    {
        using type = typename tlist_dedupe_impl<tlist<T...>, tlist<>>::type;
    };

    template <typename T>
    using tlist_dedupe_t = typename tlist_dedupe<T>::type;

    // join

    template <typename... T>
    struct tlist_join;

    template <typename... T1, typename... T2, typename... T>
    struct tlist_join<tlist<T1...>, tlist<T2...>, T...> final
    {
        using type = typename tlist_join<tlist<T1..., T2...>, T...>::type;
    };

    template <typename... T1>
    struct tlist_join<tlist<T1...>> final
    {
        using type = tlist<T1...>;
    };

    template <>
    struct tlist_join<> final
    {
        using type = tlist<>;
    };

    template <typename... T>
    using tlist_join_t = typename tlist_join<T...>::type;
}

#pragma once

#include "tlist.hpp"

#include <cstddef>
#include <tuple>
#include <utility>

namespace nil::xalt
{
    namespace detail
    {
        consteval auto pow_2(std::size_t value) -> std::size_t
        {
            return value == 0 ? 0 : 1 << (value - 1);
        }

        consteval auto make_mask(std::size_t value) -> std::size_t
        {
            return value == 0 ? pow_2(value) : pow_2(value) | make_mask(value - 1);
        }

        template <
            std::size_t M,
            typename I = std::index_sequence<>,
            typename O = std::index_sequence<>>
        struct index_masked;

        template <std::size_t M, std::size_t IA, std::size_t... I, std::size_t... O>
        struct index_masked<M, std::index_sequence<IA, I...>, std::index_sequence<O...>>
            : std::conditional_t<
                  (M & pow_2(IA + 1)) == pow_2(IA + 1),
                  index_masked<M, std::index_sequence<I...>, std::index_sequence<O..., IA>>,
                  index_masked<M, std::index_sequence<I...>, std::index_sequence<O...>>>
        {
        };

        template <std::size_t M, std::size_t... O>
        struct index_masked<M, std::index_sequence<>, std::index_sequence<O...>>
        {
            using type = std::index_sequence<O...>;
        };

        template <typename A, typename T, typename I>
        struct check_one;

        template <typename A, typename... T, std::size_t... I>
        struct check_one<A, tlist_types<T...>, std::index_sequence<I...>>
        {
            using ttype = std::tuple<T*...>;
            static constexpr auto value = requires() { A((*std::get<I>(ttype()))...); };

            static auto create(T... args)
            {
                const auto t = ttype(&args...);
                return A((*std::get<I>(t))...);
            }
        };

        template <typename A, typename T>
        struct make;

        template <typename A, typename... T>
        struct make<A, tlist_types<T...>>
        {
            template <std::size_t M>
            static auto apply(T... args)
            {
                using masked_type =
                    typename index_masked<M, std::make_index_sequence<sizeof...(T)>>::type;

                if constexpr (check_one<A, tlist_types<T...>, masked_type>::value)
                {
                    if constexpr (!value<M - 1>)
                    {
                        return check_one<A, tlist_types<T...>, masked_type>::create(args...);
                    }
                }
                else if constexpr (M > 0)
                {
                    return apply<M - 1>(args...);
                }
            }

            template <std::size_t M>
            static constexpr auto value
                = M < 64 && std::is_same_v<A, decltype(apply<M>(std::declval<T>()...))>;
        };
    }

    template <typename A, typename... T>
        requires(!detail::make<A, tlist_types<T...>>::template value<detail::make_mask(sizeof...(T)
                 )>)
    auto make(T... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<A, tlist_types<T...>>::template value<detail::make_mask(sizeof...(T)
                 )>)
    auto make(T... args)
    {
        using maker = detail::make<A, tlist_types<T...>>;
        static constexpr auto mask = detail::make_mask(sizeof...(T));
        return maker::template apply<mask>(args...);
    }
}

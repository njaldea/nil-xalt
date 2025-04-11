#pragma once

#include "tlist.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace nil::xalt::detail
{
    consteval auto pow_2(std::size_t value) -> std::size_t
    {
        return value == 0 ? 0 : 1 << (value - 1);
    }

    consteval auto make_mask(std::size_t value) -> std::size_t
    {
        return value == 0 ? pow_2(value) : pow_2(value) | make_mask(value - 1);
    }

    template <std::size_t M, typename I = std::index_sequence<>, typename O = std::index_sequence<>>
    struct index_masked;

    template <std::size_t M, std::size_t IA, std::size_t... I, std::size_t... O>
    struct index_masked<M, std::index_sequence<IA, I...>, std::index_sequence<O...>>
        : std::conditional_t<
              (pow_2(IA + 1) & M) == pow_2(IA + 1),
              index_masked<M, std::index_sequence<I...>, std::index_sequence<IA, O...>>,
              index_masked<M, std::index_sequence<I...>, std::index_sequence<O...>>>
    {
    };

    template <std::size_t M, std::size_t... O>
    struct index_masked<M, std::index_sequence<>, std::index_sequence<O...>>
    {
        using type = std::index_sequence<O...>;
    };

    template <std::size_t M, std::size_t I>
    using index_masked_t = typename index_masked<M, std::make_index_sequence<I>>::type;

    template <typename T>
    struct type_caster
    {
        // NOLINTNEXTLINE
        T cast() const
        {
            return static_cast<T>(*ptr);
        }

        std::remove_reference_t<T>* ptr;
    };

    template <typename A, typename T, typename I>
    struct check_one;

    template <typename A, typename... T, std::size_t... I>
    struct check_one<A, tlist_types<T...>, std::index_sequence<I...>>
    {
        static auto create(T... args)
        {
            const auto t = std::make_tuple(type_caster<T>{&args}...);
            constexpr auto i = sizeof...(T) - 1;
            if constexpr (requires() { A(std::get<i - I>(t).cast()...); })
            {
                return A(std::get<i - I>(t).cast()...);
            }
        }
    };

    template <typename A, typename T>
    struct create;

    template <typename A, typename... T>
    struct create<A, tlist_types<T...>>
    {
        template <std::size_t M>
        static auto apply(T... args)
        {
            using masked_type = index_masked_t<M, sizeof...(T)>;
            using ctype = check_one<A, tlist_types<T...>, masked_type>;
            if constexpr (!std::is_same_v<void, decltype(ctype::create(std::declval<T>()...))>)
            {
                return ctype::create(std::forward<T>(args)...);
            }
            else if constexpr (M > 0)
            {
                return apply<M - 1>(std::forward<T>(args)...);
            }
        }
    };

    template <typename A, typename... T>
    auto fn_make(T&&... args)
    {
        using create_t = create<A, tlist_types<T&&...>>;
        static constexpr auto mask = detail::make_mask(sizeof...(T));
        return create_t::template apply<mask>(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
    constexpr auto fn_make_v
        = !std::is_same_v<void, decltype(detail::fn_make<A>(std::declval<T&&>()...))>
        && (sizeof...(T) < 64);
}

namespace nil::xalt
{
    template <typename A, typename... T>
        requires(!detail::fn_make_v<A, T...>)
    auto fn_make(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::fn_make_v<A, T...>)
    auto fn_make(T&&... args)
    {
        return detail::fn_make<A>(std::forward<T>(args)...);
    }
}

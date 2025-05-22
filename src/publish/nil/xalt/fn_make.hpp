#pragma once

#include "cast.hpp"

#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

#include "tlist_values.hpp"

namespace nil::xalt::detail
{
    template <std::size_t value>
    consteval auto make_mask() -> std::size_t
    {
        return value == 0 ? 0 : (std::size_t(1) << (value)) - 1;
    }

    struct not_hit
    {
        constexpr bool operator()(std::size_t value, std::size_t mask)
        {
            const auto bit = std::size_t(1) << value;
            return (bit & mask) == bit;
        }
    };

    struct inverse
    {
        constexpr std::size_t operator()(std::size_t value, std::size_t max)
        {
            return max - 1 - value;
        }
    };

    template <typename A, typename... Args>
    struct make;

    template <typename A, typename... Args>
    struct make
    {
    public:
        static auto call(Args... args)
        {
            return apply<detail::make_mask<sizeof...(Args)>()>(std::forward<Args>(args)...);
        }

    private:
        template <std::size_t M>
        static auto apply(Args... args)
        {
            using masked_type                                                                 //
                = typename xalt::to_tlist_values_t<std::make_index_sequence<sizeof...(Args)>> //
                ::template remove_if<not_hit, M>::template apply<inverse, sizeof...(Args)>;

            constexpr auto is_compatible
                = !std::is_same_v<void, decltype(create(masked_type(), std::declval<Args>()...))>;
            if constexpr (is_compatible)
            {
                return create(masked_type(), std::forward<Args>(args)...);
            }
            else if constexpr (M > 0)
            {
                return apply<M - 1>(std::forward<Args>(args)...);
            }
        }

        template <std::size_t... I>
        static auto create(tlist_values<I...> /* i */, Args... args)
        {
            using element_type = typename A::type;
            const auto t = std::make_tuple(explicit_cast<Args>{&args}...);
            constexpr auto is_constructible
                = requires() { element_type(std::get<sizeof...(Args) - I - 1>(t).cast()...); };
            if constexpr (is_constructible)
            {
                return A::make(std::get<sizeof...(Args) - I - 1>(t).cast()...);
            }
        }
    };

    template <typename A, typename... Args>
        requires(sizeof...(Args) >= 10)
    struct make<A, Args...>
    {
        static_assert(sizeof...(Args) < 10); // only use with less than 10 arguments
        static auto call(Args... args) = delete;
    };

    template <typename A, typename... Args>
    concept maker_found
        = !std::is_same_v<void, decltype(make<A, Args...>::call(std::declval<Args>()...))>;
}

namespace nil::xalt
{
    template <typename A>
    struct fn_make_strategy
    {
        using type = A;

        template <typename... T>
        static auto make(T&&... args)
        {
            return A(std::forward<T>(args)...);
        }
    };

    template <typename A>
    struct fn_make_strategy<std::unique_ptr<A>>
    {
        using type = A;

        template <typename... T>
        static auto make(T&&... args)
        {
            return std::make_unique<A>(std::forward<T>(args)...);
        }
    };

    template <typename A>
    struct fn_make_strategy<std::shared_ptr<A>>
    {
        using type = A;

        template <typename... T>
        static auto make(T&&... args)
        {
            return std::make_shared<A>(std::forward<T>(args)...);
        }
    };

    template <typename A, typename... T>
        requires(!detail::maker_found<fn_make_strategy<A>, T && ...>)
    auto fn_make(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::maker_found<fn_make_strategy<A>, T && ...>)
    auto fn_make(T&&... args)
    {
        return detail::make<fn_make_strategy<A>, T&&...>::call(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
    auto fn_make_unique(T&&... args)
    {
        return fn_make<std::unique_ptr<A>>(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
    auto fn_make_shared(T&&... args)
    {
        return fn_make<std::shared_ptr<A>>(std::forward<T>(args)...);
    }
}

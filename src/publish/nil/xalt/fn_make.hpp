#pragma once

#include "cast.hpp"

#include <cstddef>
#include <memory>
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

    template <typename A, typename... Args>
    struct make
    {
    public:
        static auto call(Args... args)
        {
            static constexpr auto mask = detail::make_mask(sizeof...(Args));
            return apply<mask>(std::forward<Args>(args)...);
        }

        static constexpr bool value = (sizeof...(Args) < 64)
            && !std::is_same_v<void, decltype(call(std::declval<Args>()...))>;

    private:
        template <std::size_t M>
        static auto apply(Args... args)
        {
            using masked_type = index_masked_t<M, sizeof...(Args)>;
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
        static auto create(std::index_sequence<I...> /* i */, Args... args)
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
        requires(!detail::make<fn_make_strategy<A>, T && ...>::value)
    auto fn_make(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<fn_make_strategy<A>, T && ...>::value)
    auto fn_make(T&&... args)
    {
        using type = detail::make<fn_make_strategy<A>, T&&...>;
        return type::call(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
        requires(!detail::make<fn_make_strategy<std::unique_ptr<A>>, T && ...>::value)
    auto fn_make_unique(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<fn_make_strategy<std::unique_ptr<A>>, T && ...>::value)
    auto fn_make_unique(T&&... args)
    {
        using type = detail::make<fn_make_strategy<std::unique_ptr<A>>, T&&...>;
        return type::call(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
        requires(!detail::make<fn_make_strategy<std::shared_ptr<A>>, T && ...>::value)
    auto fn_make_shared(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<fn_make_strategy<std::shared_ptr<A>>, T && ...>::value)
    auto fn_make_shared(T&&... args)
    {
        using type = detail::make<fn_make_strategy<std::shared_ptr<A>>, T&&...>;
        return type::call(std::forward<T>(args)...);
    }
}

#pragma once

#include "cast.hpp"
#include "concepts.hpp"

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
    private:
        static constexpr auto size = sizeof...(Args);

    public:
        static auto call(Args... args)
        {
            static constexpr auto mask = detail::make_mask(size);
            return apply<mask>(std::forward<Args>(args)...);
        }

        static constexpr bool value
            = (size < 64) && !std::is_same_v<void, decltype(call(std::declval<Args>()...))>;

    private:
        template <std::size_t M>
        static auto apply(Args... args)
        {
            using masked_type = index_masked_t<M, size>;
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
            const auto t = std::make_tuple(explicit_cast<Args>{&args}...);
            constexpr auto i = size - 1;
            if constexpr (requires() { A::element_type; })
            {
                using element_type = typename A::element_type;
                if constexpr (is_of_template_v<A, std::unique_ptr>)
                {
                    return std::make_unique<element_type>(std::get<i - I>(t).cast()...);
                }
                else if constexpr (is_of_template_v<A, std::shared_ptr>)
                {
                    return std::make_shared<element_type>(std::get<i - I>(t).cast()...);
                }
            }
            else
            {
                if constexpr (requires() { A(std::get<i - I>(t).cast()...); })
                {
                    return A(std::get<i - I>(t).cast()...);
                }
            }
        }
    };
}

namespace nil::xalt
{
    template <typename A, typename... T>
        requires(!detail::make<A, T && ...>::value)
    auto fn_make(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<A, T && ...>::value)
    auto fn_make(T&&... args)
    {
        return detail::make<A, T&&...>::call(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
        requires(!detail::make<A, T && ...>::value)
    auto fn_make_unique(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<A, T && ...>::value)
    auto fn_make_unique(T&&... args)
    {
        return detail::make<std::unique_ptr<A>, T&&...>::call(std::forward<T>(args)...);
    }

    template <typename A, typename... T>
        requires(!detail::make<A, T && ...>::value)
    auto fn_make_shared(T&&... args) = delete;

    template <typename A, typename... T>
        requires(detail::make<A, T && ...>::value)
    auto fn_make_shared(T&&... args)
    {
        return detail::make<std::shared_ptr<A>, T&&...>::call(std::forward<T>(args)...);
    }
}

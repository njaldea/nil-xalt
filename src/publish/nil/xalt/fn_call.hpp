#pragma once

#include "cast.hpp"
#include "fn_sign.hpp"
#include "tlist.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace nil::xalt::detail
{
    struct invalid final
    {
    };

    template <typename A, typename... Args>
    struct fn_impl final
    {
    public:
        static auto call(Args... args)
        {
            return select<0>(std::forward<Args>(args)...);
        }

    private:
        template <std::size_t Next, std::size_t... I>
        static auto select(Args... args)
        {
            if constexpr (Next == sizeof...(Args))
            {
                if constexpr (check(tlist<typify<I>...>()))
                {
                    return call(tlist<typify<I>...>(), std::forward<Args>(args)...);
                }
                else
                {
                    return invalid();
                }
            }
            else
            {
                using selected_type
                    = decltype(select<Next + 1, I..., Next>(std::forward<Args>(args)...));

                if constexpr (!std::is_same_v<selected_type, invalid>)
                {
                    return select<Next + 1, I..., Next>(std::forward<Args>(args)...);
                }
                else
                {
                    return select<Next + 1, I...>(std::forward<Args>(args)...);
                }
            }
        }

        template <std::size_t... I>
        static auto call(tlist<typify<I>...> /* i */, Args... args)
        {
            const auto t = std::make_tuple(explicit_cast<Args>{&args}...);
            return A::call(std::get<I>(t).cast()...);
        }

        template <std::size_t... I>
        static constexpr auto check(tlist<typify<I>...> /* i */)
        {
            using arg_list = tlist<Args...>;
            return A::template check<typename arg_list::template at<I>...>;
        }
    };

    template <typename A, typename... Args>
        requires(sizeof...(Args) >= 10)
    struct fn_impl<A, Args...> final
    {
        static_assert(sizeof...(Args) < 10); // only use with less than 10 arguments
        static auto call(Args... args) = delete;
    };

    template <typename A, typename... Args>
    concept fn_impl_found
        = !std::is_same_v<invalid, decltype(fn_impl<A, Args...>::call(std::declval<Args>()...))>;
}

namespace nil::xalt
{
    template <typename A>
    struct fn_strategy;

    template <typename A>
    struct fn_strategy<fn_sign<A>>
    {
        template <typename S, typename... T>
        static constexpr auto check
            = requires(S s) { s(std::declval<xalt::implicit_cast<T>>()...); };

        template <typename... T>
        static auto call(A&& callable, T&&... args)
        {
            return callable(std::forward<T>(args)...);
        }
    };

    template <typename A, typename... T>
    auto fn_call(A&& callable, T&&... args)
    {
        return detail::fn_impl<fn_strategy<fn_sign<A>>, A&&, T&&...>::call(
            std::forward<A>(callable),
            std::forward<T>(args)...
        );
    }

    template <auto A, typename... T>
        requires(!is_fn<decltype(A)>)
    auto fn_call(T&&... args) = delete;
}

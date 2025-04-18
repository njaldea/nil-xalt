#pragma once

#include "tlist.hpp"

namespace nil::xalt
{
    template <typename T>
    struct fn_sign final
    {
        using class_type = fn_sign<decltype(&T::operator())>::class_type;
        using return_type = fn_sign<decltype(&T::operator())>::return_type;
        using arg_types = fn_sign<decltype(&T::operator())>::arg_types;
        static constexpr auto is_const = fn_sign<decltype(&T::operator())>::is_const;
        static constexpr auto is_noexcept = fn_sign<decltype(&T::operator())>::is_noexcept;
    };

    template <typename R, typename... Args>
    struct fn_sign<R(Args...)> final
    {
        using class_type = void;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = false;
        static constexpr auto is_noexcept = false;
    };

    template <typename R, typename... Args>
    struct fn_sign<R(Args...) noexcept> final
    {
        using class_type = void;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = false;
        static constexpr auto is_noexcept = true;
    };

    template <typename C, typename R, typename... Args>
    struct fn_sign<R (C::*)(Args...)> final
    {
        using class_type = C;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = false;
        static constexpr auto is_noexcept = false;
    };

    template <typename C, typename R, typename... Args>
    struct fn_sign<R (C::*)(Args...) const> final
    {
        using class_type = C;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = false;
    };

    template <typename C, typename R, typename... Args>
    struct fn_sign<R (C::*)(Args...) noexcept> final
    {
        using class_type = C;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = false;
        static constexpr auto is_noexcept = true;
    };

    template <typename C, typename R, typename... Args>
    struct fn_sign<R (C::*)(Args...) const noexcept> final
    {
        using class_type = C;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = true;
    };

    template <typename R, typename... Args>
    struct fn_sign<R (*)(Args...)> final
    {
        using class_type = void;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = false;
    };

    template <typename R, typename... Args>
    struct fn_sign<R (*)(Args...) noexcept> final
    {
        using class_type = void;
        using return_type = R;
        using arg_types = tlist_types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = true;
    };
}

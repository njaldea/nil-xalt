#pragma once

namespace nil::xalt
{
    template <typename... T>
    struct types;

    template <typename T>
    struct fsign final
    {
        using class_name = fsign<decltype(&T::operator())>::class_name;
        using return_type = fsign<decltype(&T::operator())>::return_type;
        using arg_types = fsign<decltype(&T::operator())>::arg_types;
        static constexpr auto is_const = fsign<decltype(&T::operator())>::is_const;
        static constexpr auto is_noexcept = fsign<decltype(&T::operator())>::is_noexcept;
    };

    template <typename C, typename R, typename... Args>
    struct fsign<R (C::*)(Args...)> final
    {
        using class_name = C;
        using return_type = R;
        using arg_types = types<Args...>;
        static constexpr auto is_const = false;
        static constexpr auto is_noexcept = false;
    };

    template <typename C, typename R, typename... Args>
    struct fsign<R (C::*)(Args...) const> final
    {
        using class_name = C;
        using return_type = R;
        using arg_types = types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = false;
    };

    template <typename C, typename R, typename... Args>
    struct fsign<R (C::*)(Args...) noexcept> final
    {
        using class_name = C;
        using return_type = R;
        using arg_types = types<Args...>;
        static constexpr auto is_const = false;
        static constexpr auto is_noexcept = true;
    };

    template <typename C, typename R, typename... Args>
    struct fsign<R (C::*)(Args...) const noexcept> final
    {
        using class_name = C;
        using return_type = R;
        using arg_types = types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = true;
    };

    template <typename R, typename... Args>
    struct fsign<R (*)(Args...)> final
    {
        using class_name = void;
        using return_type = R;
        using arg_types = types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = false;
    };

    template <typename R, typename... Args>
    struct fsign<R (*)(Args...) noexcept> final
    {
        using class_name = void;
        using return_type = R;
        using arg_types = types<Args...>;
        static constexpr auto is_const = true;
        static constexpr auto is_noexcept = true;
    };
}

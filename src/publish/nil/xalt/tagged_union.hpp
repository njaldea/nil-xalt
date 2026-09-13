// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "typed.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <new>
#include <tuple>
#include <type_traits>
#include <utility>

namespace nil::xalt
{
    // A tagged union over a small, closed set of trivially-copyable alternatives. A
    // lighter-weight alternative to std::variant/std::visit: since every alternative is
    // trivially copyable, copy/move/destroy are implicit (bitwise) and dispatch is a
    // compile-time fold over Ts... instead of std::visit's recursive vtable-generation
    // machinery, which is a measurable compile-time/memory win for small variant-like types
    // used on hot paths.
    template <typename... Ts>
    class tagged_union
    {
        static_assert(sizeof...(Ts) > 0, "tagged_union requires at least one alternative");
        static_assert(
            (std::is_trivially_copyable_v<Ts> && ...),
            "tagged_union alternatives must be trivially copyable"
        );

        static constexpr std::size_t storage_size = (std::max)({sizeof(Ts)...});
        static constexpr std::size_t storage_align = (std::max)({alignof(Ts)...});

        const void* id = nullptr;

        alignas(storage_align) std::array<std::byte, storage_size> storage = {};

        template <typename T>
        static constexpr bool is_alternative = (std::is_same_v<T, Ts> || ...);

        using first_t = std::tuple_element_t<0, std::tuple<Ts...>>;

    public:
        tagged_union() = default;

        template <typename T>
            requires(is_alternative<T>)
        explicit tagged_union(T value) // NOLINT(google-explicit-constructor)
            : id(type_id<T>)
        {
            ::new (static_cast<void*>(storage.data())) T(value);
        }

        // Every alternative is trivially copyable, so a bitwise copy of storage/id is a
        // correct copy/move of whichever alternative (if any) is currently active.
        tagged_union(const tagged_union&) = default;
        tagged_union(tagged_union&&) = default;
        tagged_union& operator=(const tagged_union&) = default;
        tagged_union& operator=(tagged_union&&) = default;
        ~tagged_union() = default;

        bool is_empty() const
        {
            return id == nullptr;
        }

        template <typename T>
        bool holds() const
        {
            return id == type_id<T>;
        }

        template <typename T>
        T& get()
        {
            return *std::launder(reinterpret_cast<T*>(storage.data())); // NOLINT
        }

        template <typename T>
        const T& get() const
        {
            return *std::launder(reinterpret_cast<const T*>(storage.data())); // NOLINT
        }

        template <typename Visitor>
        decltype(auto) visit(Visitor&& vis)
        {
            using R = decltype(vis(std::declval<first_t&>()));
            if constexpr (std::is_void_v<R>)
            {
                (void)((id == type_id<Ts> ? (vis(get<Ts>()), true) : false) || ...);
            }
            else
            {
                R result{};
                (void)((id == type_id<Ts> ? (result = vis(get<Ts>()), true) : false) || ...);
                return result;
            }
        }
    };
}

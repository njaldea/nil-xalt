// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#pragma once

namespace nil::xalt
{
    template <auto V>
    struct typify final
    {
        static constexpr auto value = V;
    };

    template <typename T>
    inline constexpr char type_id_tag = '\0';

    template <typename T>
    inline constexpr const void* type_id = &type_id_tag<T>;
}

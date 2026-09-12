// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#include <nil/xalt/str_enum.hpp>

namespace bench
{
    enum class Dense0
    {
        V0 = 0,
        V1,
        V2,
        V3,
        V4,
        V5,
        V6,
        V7,
        V8,
        V9,
        V10,
        V11,
        V12,
        V13,
        V14,
        V15,
        V16,
        V17,
        V18,
        V19
    };

    enum class Dense10
    {
        V10 = 1,
        V11,
        V12,
        V13,
        V14,
        V15,
        V16,
        V17,
        V18,
        V19,
        V20,
        V21,
        V22,
        V23,
        V24,
        V25,
        V26,
        V27,
        V28,
        V29
    };

    enum class Masked
    {
        B0 = 1 << 0,
        B1 = 1 << 1,
        B2 = 1 << 2,
        B3 = 1 << 3,
        B4 = 1 << 4,
        B5 = 1 << 5,
        B6 = 1 << 6,
        B7 = 1 << 7,
        B8 = 1 << 8,
        B9 = 1 << 9
    };

    template <typename E>
    consteval auto touch_enum() -> bool
    {
        using nil::xalt::str_enum_values_t;

        using values = str_enum_values_t<E>;
        return values::size > 0;
    }

    static_assert(touch_enum<Dense0>());
    static_assert(touch_enum<Dense10>());
    static_assert(touch_enum<Masked>());

    // Force name materialization for all discovered values.
    static_assert(nil::xalt::str_name_v<nil::xalt::typify<Dense0::V0>>[0] != '\0');
    static_assert(nil::xalt::str_name_v<nil::xalt::typify<Dense0::V19>>[0] != '\0');
    static_assert(nil::xalt::str_name_v<nil::xalt::typify<Dense10::V10>>[0] != '\0');
    static_assert(nil::xalt::str_name_v<nil::xalt::typify<Dense10::V29>>[0] != '\0');
    static_assert(nil::xalt::str_name_v<nil::xalt::typify<Masked::B0>>[0] != '\0');
    static_assert(nil::xalt::str_name_v<nil::xalt::typify<Masked::B9>>[0] != '\0');
}

int main()
{
    return 0;
}

#pragma once

namespace nil::xalt
{
    template <typename T>
    inline constexpr char type_id_tag = '\0';

    template <typename T>
    inline constexpr const void* type_id = &type_id_tag<T>;
}

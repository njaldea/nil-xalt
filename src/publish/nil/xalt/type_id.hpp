#pragma once

namespace nil::xalt
{
    template <typename T>
    struct type_id_tag final
    {
        static constexpr char value = '\0';
    };

    template <typename T>
    struct type_id final
    {
        static constexpr const void* value = &type_id_tag<T>::value;
    };

    template <typename T>
    static constexpr const void* type_id_v = type_id<T>::value;
}

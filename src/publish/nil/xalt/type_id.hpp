#pragma once

namespace nil::xalt
{
    template <typename T>
    struct type_id_tag
    {
        static constexpr char value = '\0';
    };

    template <typename T>
    struct type_id
    {
        static constexpr const void* value = &type_id_tag<T>::value;
    };

    template <typename T>
    static constexpr auto type_id_v = type_id<T>::value;
}

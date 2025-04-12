#pragma once

#include <type_traits>

namespace nil::xalt
{
    template <typename T>
    struct explicit_cast
    {
        T cast() const
        {
            return static_cast<T>(*ptr);
        }

        std::remove_reference_t<T>* ptr;
    };

    template <typename T>
    struct implicit_cast
    {
        template <typename U>
        // NOLINTNEXTLINE
        operator U&&() const
        {
            return static_cast<U&&>(*ptr);
        }

        std::remove_reference_t<T>* ptr;
    };
}

#pragma once

#include <type_traits>

namespace nil::xalt
{
    template <typename T>
    struct explicit_cast final
    {
        T cast() const
        {
            return static_cast<T>(*ptr);
        }

        std::remove_reference_t<T>* ptr;
    };

    template <typename T>
    struct implicit_cast final
    {
        template <typename U>
        // NOLINTNEXTLINE(hicpp-explicit-conversions)
        operator U&&() const
        {
            return static_cast<U&&>(*ptr);
        }

        std::remove_reference_t<T>* ptr;
    };
}

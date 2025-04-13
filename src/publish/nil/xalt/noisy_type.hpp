#pragma once

#include "literal.hpp"
#include "str_name.hpp"

#include <cstdio>

namespace nil::xalt
{
    template <literal lit, typename... T>
    struct noisy_type final
    {
        explicit noisy_type(T&&... /*t*/)
        {
            std::printf("%s: constructor\n", literal_v<lit>);
            (std::printf("  - %s\n", str_name_type_v<T>), ...);
        }

        ~noisy_type()
        {
            std::printf("%s: destructor\n", literal_v<lit>);
        }

        noisy_type(const noisy_type& /* other*/)
        {
            std::printf("%s: copy constructor\n", literal_v<lit>);
        }

        noisy_type(noisy_type&& /* other */) noexcept
        {
            std::printf("%s: move constructor\n", literal_v<lit>);
        }

        noisy_type& operator=(const noisy_type& /* other */)
        {
            std::printf("%s: copy assignment\n", literal_v<lit>);
            return *this;
        }

        noisy_type& operator=(noisy_type&& /* other */) noexcept
        {
            std::printf("%s: move assignment\n", literal_v<lit>);
            return *this;
        }
    };
}

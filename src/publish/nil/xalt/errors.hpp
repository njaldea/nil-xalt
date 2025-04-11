#pragma once

namespace nil::xalt
{
    template <typename... T>
    void unreachable() = delete;

    template <auto... T>
    void unreachable() = delete;
}

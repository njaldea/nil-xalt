// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#include <nil/xalt/str_name.hpp>

#include <type_traits>

namespace bench
{
    template <typename T>
    struct Box
    {
        template <typename U>
        struct Inner;
    };

    template <typename T>
    template <typename U>
    struct Box<T>::Inner
    {
    };

    template <int N>
    struct Tag
    {
    };

    template <int N>
    using T0 = Tag<N>;

    template <int N>
    using T1 = Box<T0<N>>;

    template <int N>
    using T2 = Box<T1<N>>;

    template <int N>
    using T3 = Box<T2<N>>;

    template <int N>
    using T4 = typename T1<N>::template Inner<T3<N>>;

    template <typename T>
    consteval auto touch() -> bool
    {
        using nil::xalt::str_name;
        using nil::xalt::str_scope_name;
        using nil::xalt::str_short_base_name;
        using nil::xalt::str_short_name;

        constexpr auto n0 = str_name<T>();
        constexpr auto n1 = str_short_name<T>();
        constexpr auto n2 = str_scope_name<T>();
        constexpr auto n3 = str_short_base_name<T>();

        // Consume all names to force full compile-time instantiation.
        return sizeof(n0) + sizeof(n1) + sizeof(n2) + sizeof(n3) > 0;
    }

#define NIL_XALT_BENCH_ONE(N)                                                                      \
    static_assert(touch<T0<N>>());                                                                 \
    static_assert(touch<T1<N>>());                                                                 \
    static_assert(touch<T2<N>>());                                                                 \
    static_assert(touch<T3<N>>());                                                                 \
    static_assert(touch<T4<N>>())

    NIL_XALT_BENCH_ONE(0);
    NIL_XALT_BENCH_ONE(1);
    NIL_XALT_BENCH_ONE(2);
    NIL_XALT_BENCH_ONE(3);
    NIL_XALT_BENCH_ONE(4);
    NIL_XALT_BENCH_ONE(5);
    NIL_XALT_BENCH_ONE(6);
    NIL_XALT_BENCH_ONE(7);
    NIL_XALT_BENCH_ONE(8);
    NIL_XALT_BENCH_ONE(9);
    NIL_XALT_BENCH_ONE(10);
    NIL_XALT_BENCH_ONE(11);
    NIL_XALT_BENCH_ONE(12);
    NIL_XALT_BENCH_ONE(13);
    NIL_XALT_BENCH_ONE(14);
    NIL_XALT_BENCH_ONE(15);
    NIL_XALT_BENCH_ONE(16);
    NIL_XALT_BENCH_ONE(17);
    NIL_XALT_BENCH_ONE(18);
    NIL_XALT_BENCH_ONE(19);

#undef NIL_XALT_BENCH_ONE
} // namespace bench

int main()
{
    return 0;
}

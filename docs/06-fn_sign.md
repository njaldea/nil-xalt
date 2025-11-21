# fn_sign

## nil::xalt::fn_sign

```cpp
int foo()
{
    // Prefer the function type (not pointer) to reflect the signature directly
    using traits = nil::xalt::fn_sign<decltype(foo)>;

    using free_type   = traits::free_type;      // R(Args...) -- int()
    using return_type = traits::return_type;    // R          -- int
    using arg_types   = traits::arg_types;      // (Args...)  -- tlist<Args...>;
    using class_type  = traits::class_type;     // void if not a member function

    static_assert(!traits::is_const);
    static_assert(!traits::is_noexcept);
}
```

Notes:
- Function types (e.g., `decltype(foo)`) report `is_const == false`.
- Free function pointers (e.g., `decltype(&foo)`) now also report `is_const == false`.
- `free_type` preserves `noexcept` when present.

More examples:

```cpp
struct C {
    int m(double) const noexcept;    // const, noexcept member
};

using M = nil::xalt::fn_sign<decltype(&C::m)>;
static_assert(std::is_same_v<M::class_type, C>);
static_assert(M::is_const);
static_assert(M::is_noexcept);
static_assert(std::is_same_v<M::free_type, int(double) noexcept>);

// Free function pointer: function type and pointer now agree on const flag
int bar(float) noexcept;
using F1 = nil::xalt::fn_sign<decltype(bar)>;      // function type
using F2 = nil::xalt::fn_sign<decltype(&bar)>;     // pointer-to-function
static_assert(!F1::is_const);
static_assert(!F2::is_const);
static_assert(F1::is_noexcept && F2::is_noexcept);
```

## Dependencies

- [tlist](./03-tlist.md)

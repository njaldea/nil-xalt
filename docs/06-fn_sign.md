# fn_sign

Contains a type trait for inspecting information from any callable types, including both free and member functions.

## nil::xalt::fn_sign

```cpp
int foo()
{
    using traits = nil::xalt::fn_sign<decltype(&foo)>;

    using free_type   = traits::free_type;      // R(Args...) -- void()
    using return_type = traits::return_type;    // R          -- void
    using arg_types   = traits::arg_types;      // (Args...)  -- tlist<Args...>;
    using class_type  = traits::class_type;     // void if not a member function

    static_assert(!traits::is_const);
    static_assert(!traits::is_noexcept);
}
```

## Dependencies

- [tlist](./04-tlist.md)

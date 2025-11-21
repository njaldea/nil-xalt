# str_name

Provides compile-time utilities to extract the string representation of types and constant values.

```cpp
namespace Namespace
{
    struct Class
    {
        struct YourType;
    };
}
```

## nil::xalt::str_name

Returns the full qualified name of the type.

```cpp
int main()
{
    // Will return "Namespace::Class::YourType"

    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_name_v<Namespace::Class::YourType>);
    // or string_view
    std::puts(nil::xalt::str_name_sv<Namespace::Class::YourType>.data());
}
```

## nil::xalt::str_short_name

Strips away the namespace or class the type belongs to.

```cpp
int main()
{
    // Will return "YourType"

    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_short_name_v<Namespace::Class::YourType>);
    // or string_view
    std::puts(nil::xalt::str_short_name_sv<Namespace::Class::YourType>.data());
}
```

## nil::xalt::str_scope_name

Returns only the namespace or class the type belongs to.

```cpp
int main()
{
    // Will return "Namespace::Class"

    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_scope_name_v<Namespace::Class::YourType>);
    // or string_view
    std::puts(nil::xalt::str_scope_name_sv<Namespace::Class::YourType>.data());
}
```

## typify

When using [typify](./10-typed.md#nilxalttypify), the str_name functions will extract the underlying value:

```cpp
static_assert(str_name_sv<typify<Status::Active>> == "Status::Active");
static_assert(str_short_name_sv<typify<Status::Active>> == "Active");
static_assert(str_scope_name_sv<typify<Status::Active>> == "Status");

// Integer values
static_assert(str_name_sv<typify<42>> == "42");

// Enum values with explicit casting
static_assert(str_name_sv<typify<Status(4)>> == "(Status)4");
```

## Compiler Support

This functionality requires compiler-specific intrinsics:
- **GCC/Clang**: Uses `__PRETTY_FUNCTION__`  
- **MSVC**: Uses `__FUNCSIG__`

If the compiler doesn't support these intrinsics, the functions will not be available and compilation will fail with appropriate error messages from the [errors](./11-errors.md) module.

## Dependencies

- [literal](./02-literal.md) - For compile-time string manipulation
- [typed](./10-typed.md) - For `typify` support
- [errors](./11-errors.md) - For unsupported compiler diagnostics

## Notes

- All operations are performed at compile-time with zero runtime overhead
- The output format may vary slightly between different compilers
- Template parameters in type names are fully expanded
- Nested class names include their containing scope
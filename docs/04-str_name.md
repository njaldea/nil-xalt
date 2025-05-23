# str_name

Provides compile-time utilities to extract the string representation of types and constant values.

## nil::xalt::str_name

```cpp
int main()
{
    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_name_v<YourType>);
    // or string_view
    std::puts(nil::xalt::str_name_sv<YourType>.data());
}
```

### Dependencies

- [literal](./02-literal.md)
- [typed](./09-typed.md)

This requires `__PRETTY_FUNCTION__` or `__FUNCSIG__`

If not supported:
- [str_name](./11-errors.md)
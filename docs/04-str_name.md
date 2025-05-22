# str_name

Provides compile-time utilities to extract the string representation of types and constant values.

## nil::xalt::str_name_type

```cpp
int main()
{
    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_name_type_v<YourType>);
    // or string_view
    std::puts(nil::xalt::str_name_type_sv<YourType>.data());
}
```

## nil::xalt::str_name_value

```cpp
int main()
{
    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_name_value_v<100>);
    // or string_view
    std::puts(nil::xalt::str_name_value_v<100>.data());

    // useful for getting names of enums
}
```

### Requirements

This requires `__PRETTY_FUNCTION__` or `__FUNCSIG__`

If not supported:
- [str_name](./06-str_name.md)
# noisy_type

Provides a utility type that logs when its constructors and other special member functions are invoked.

## nil::xalt::noisy_type

```cpp
int main()
{
    auto noisy_object
        = nil::xalt::noisy_type<"my_type", int, bool>();
    // "my_type: constructor"
    // "  - int"
    // "  - bool"

    auto noisy_object_copy = noisy_object;
    // "my_type: copy constructor"

    // and all other special member functions
}
```

## Dependencies

- [literal](./03-literal.md)
- [str_name](./06-str_name.md)

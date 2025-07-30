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

## Tracked Operations

The following special member functions are tracked and logged:

- Default constructor
- Copy constructor  
- Move constructor
- Copy assignment operator
- Move assignment operator
- Destructor
- Parameterized constructors (with argument type names)

## Dependencies

- [literal](./02-literal.md)
- [str_name](./04-str_name.md)

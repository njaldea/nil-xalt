# noisy_type

Lightweight utility that logs when its constructors and special member functions are invoked.

Includes: `#include <nil/xalt/noisy_type.hpp>`

## nil::xalt::noisy_type

```cpp
int main()
{
    // Name is a compile-time literal; template parameters list expected ctor arg types
    using my_noisy_t = nil::xalt::noisy_type<"my_type", int, bool>;

    my_noisy_t a;              // prints: "my_type: constructor"
    my_noisy_t b{1, true};     // prints: 
                               //   my_type: constructor
                               //   - int : 1
                               //   - bool : 1

    auto c = b;                // prints: "my_type: copy constructor"
    auto d = std::move(b);     // prints: "my_type: move constructor"
    c = d;                     // prints: "my_type: copy assignment"
    d = std::move(c);          // prints: "my_type: move assignment"
}                              // prints on scope exit: "my_type: destructor" for each
```

Notes:
- Argument values are printed via `operator<<(std::ostream&, value)`, so they must be streamable.
- Type names are derived using `str_name`, and the label uses the `literal` template parameter.

## Tracked Operations

The following special member functions are tracked and logged:

- Default constructor
- Destructor
- Copy constructor
- Move constructor
- Copy assignment
- Move assignment

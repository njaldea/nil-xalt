# fn_make

Smart constructor utilities that attempt multiple constructor overloads with different argument combinations.

Includes: `#include <nil/xalt/fn_make.hpp>`

## Overview

`fn_make` provides functions that try to construct objects by testing different subsets of provided arguments.

## nil::xalt::fn_make

Creates an object by trying different argument combinations until a valid constructor is found.

```cpp
#include <nil/xalt/fn_make.hpp>

struct MyClass {
    MyClass(int);
    MyClass(int, const std::string&);
    MyClass(int, const std::string&, bool);
    // No constructor taking all arguments
};

int main() {
    using namespace nil::xalt;
    
    // Will try constructors in order, skipping arguments that don't work
    auto obj = fn_make<MyClass>(42, std::string("hello"), true, 3.14, "extra");
    // Finds: MyClass(int, const std::string&, bool) constructor
}
```

## nil::xalt::fn_make_unique

Creates a `std::unique_ptr` using the same argument-matching logic:

```cpp
auto ptr = fn_make_unique<MyClass>(42, "hello", true, 3.14);
// Returns: std::unique_ptr<MyClass>
```

## nil::xalt::fn_make_shared

Creates a `std::shared_ptr` using the same argument-matching logic:

```cpp
auto ptr = fn_make_shared<MyClass>(42, "hello", true, 3.14);
// Returns: std::shared_ptr<MyClass>
```

## How It Works

The algorithm uses bit masking to try different subsets:
1. Start from the full argument set.
2. Scan subsets in descending mask order.
3. For each subset, probe constructibility with `implicit_cast`.
4. Construct with original forwarded arguments for the first successful subset.

## Dependencies

- [fn_call](./07-fn_call.md) - Argument subset search and invocation engine
- [cast](./12-cast.md) - For argument type conversion during probing
- [tlist](./03-tlist.md) - For argument type manipulation

## Notes

- All operations are performed at compile-time where possible
- Throws compilation error if no valid constructor can be found
- Subset preference follows descending mask order
- Useful for generic factory functions and forwarding constructors

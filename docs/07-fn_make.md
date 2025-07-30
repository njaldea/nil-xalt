# fn_make

Smart constructor utilities that attempt multiple constructor overloads with different argument combinations.

## Overview

`fn_make` provides functions that try to construct objects by testing different combinations of provided arguments, automatically finding the best matching constructor.

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

The algorithm uses bit masking to try different combinations:
1. Start with all provided arguments
2. If no constructor matches, remove the last argument and try again  
3. Continue until a valid constructor is found
4. Uses compile-time argument filtering to test combinations efficiently

## Dependencies

- [cast](./11-cast.md) - For argument type conversion
- [tlist](./03-tlist.md) - For argument type manipulation

## Notes

- All operations are performed at compile-time where possible
- Throws compilation error if no valid constructor can be found
- Arguments are tested in reverse order (last arguments removed first)
- Useful for generic factory functions and forwarding constructors
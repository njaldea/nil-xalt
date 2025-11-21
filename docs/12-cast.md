# cast

Small helpers to perform controlled casts in generic code without changing call sites. Both wrappers hold a pointer to an existing object and expose a cast at use-time; they do not own or copy the value.

Includes: `#include <nil/xalt/cast.hpp>`

## nil::xalt::explicit_cast

Explicit-only wrapper that returns the target type via a member function.

Signature (simplified):

- `explicit_cast(std::remove_reference_t<T>* p)`
- `T cast() const`  // returns `static_cast<T>(*p)`; reference/cv in `T` are preserved

Example:

```cpp
#include <nil/xalt/cast.hpp>

int i = 42;
// explicit value cast
double d = nil::xalt::explicit_cast<double>{&i}.cast();

// reference-preserving cast
int v = 0;
int& ref = nil::xalt::explicit_cast<int&>{&v}.cast();
ref = 7; // writes to v
```

Notes:
- Uses `static_cast<T>(*ptr)`; if the cast is ill-formed, this fails at compile time.
- `ptr` must be non-null and point to a live object; lifetime is not managed.
- Copying the wrapper copies the pointer only (no ownership semantics).

## nil::xalt::implicit_cast

Single-target implicit conversion wrapper. It only implicitly converts to `T` (conversions to other types are deleted) which helps overload resolution in generic code.

Signature (simplified):

- `implicit_cast(std::remove_reference_t<T>* p)`
- `operator T() const`  // implicit; all other `operator U()` are deleted

Example:

```cpp
#include <nil/xalt/cast.hpp>

void f(double);

int i = 3;
auto v = nil::xalt::implicit_cast<double>{&i};
f(v); // OK: implicitly converts only to double
```

Notes:
- Same lifetime and nullability caveats as `explicit_cast`.
- Restricting conversions avoids accidental implicit conversions to unintended types in templates.

## Where it’s used

These wrappers are designed to be used by combinators like `fn_make` to try constructor candidates by selectively casting arguments during compile-time checks, without mutating the original arguments.
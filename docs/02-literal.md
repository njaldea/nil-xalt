# literal

Contains utility templates for allowing template string literals.

Also provides utility methods to inspect and manipulate literals at compile time.


Includes: `#include <nil/xalt/literal.hpp>`

## nil::xalt::literal

```cpp
template <nil::xalt::literal literal>
struct Object
{
    void print()
    {
        // fixed raw array (const char[N])
        std::puts(nil::xalt::literal_v<literal>);
        // or string_view
        std::puts(nil::xalt::literal_sv<literal>.data());
    }
};

int main()
{
    Object<"your_literal"> object;
    object.print();
}
```

## operations

| method      | arguments                                       |
|-------------|-------------------------------------------------|
| concat      | `<literal...>`                                  |
| substr      | `<literal, offset, size>`                       |
| find        | `<literal from, literal to_find>`               |
| rfind       | `<literal from, literal to_find>`               |
| starts_with | `<literal from, literal to_find>`               |
| ends_with   | `<literal from, literal to_find>`               |
| replace_one | `<literal base, literal from, literal to>`      |
| replace_all | `<literal base, literal from, literal to>`      |

```cpp
int main()
{
    using namespace nil::xalt;
    std::puts(literal_v<concat<"hello", " ", "world">()>);
    // "hello world"

    std::puts(literal_v<substr<"hello", 2, 3>()>);
    // "llo"

    static_assert(find<"hello", "ll">() == 2);
    static_assert(rfind<"ababa", "ba">() == 3);
    static_assert(starts_with<"hello", "he">());
    static_assert(ends_with<"hello", "lo">());

    std::puts(literal_v<replace_one<"abcdabcd", "bc", "BC">()>);
    // "aBCdabcd"

    std::puts(literal_v<replace_all<"abcdabcd", "bc", "BC">()>);
    // "aBCadBCd"
}
```

`replace_one` replaces only the first match. `replace_all` replaces every non-overlapping match.

Notes:
- For `replace_one` and `replace_all`, `from` must be non-empty.
- When no match is found, replace operations return the original literal.

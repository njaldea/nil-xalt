# literal

Contains utility templates for allowing template string literals.

Also provides utility methods to inspect and manipulate literals at compile time.

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
| find_first  | `<literal from, literal to_find>`               |
| starts_with | `<literal from, literal to_find>`               |
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

    static_assert(find_first<"hello", "ll">() == 2);
    static_assert(starts_with<"hello", "he">());

    std::puts(literal_v<replace_one<"abcdabcd", "bc", "BC">()>);
    // "aBCdabcd"

    std::puts(literal_v<replace_all<"abcdabcd", "bc", "BC">()>);
    // "aBCadBCd"
}
```

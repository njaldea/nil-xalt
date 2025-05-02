# nil/xalt

This will be a dump of utility libraries that has no place to go.

## Table of Contents

- [checks](#checks)
    - [nil::xalt::is_of_template](#nilxaltis_of_template)
- [literal](#literal)
    - [nil::xalt::literal](#nilxaltliteral)
    - [operations](#operations)
- [str_name](#str_name)
    - [nil::xalt::str_name_type](#nilxaltstr_name_type)
    - [nil::xalt::str_name_value](#nilxaltstr_name_value)
- [str_enum](#str_enum)
- [noisy_type](#noisy_type)
    - [nil::xalt::noisy_type](#nilxaltnoisy_type)
- [errors](#errors)
    - [nil::xalt::undefined](#nilxaltundefined)
- [tlist](#tlist)
    - [nil::xalt::tlist_types](#nilxalttlist_types)
    - [nil::xalt::tlist_values](#nilxalttlist_values)
- [cast](#cast)
    - [nil::xalt::explicit_cast](#nilxaltexplicit_cast)
    - [nil::xalt::implicit_cast (WIP)](#nilxaltimplicit_cast-wip)

## checks

### nil::xalt::is_of_template

```cpp
int main()
{
    static_assert(
        nil::xalt::is_of_template<
            T,          // your type
            std::tuple  // any typename <typename...>
        >::value
    );
    static_assert(nil::xalt::is_of_template_v<T, std::tuple>);
};
```

## literal

### nil::xalt::literal

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

### operations

| method      | arguments                                       |
|-------------|-------------------------------------------------|
| concat      | `<literal...>`                                  |
| substr      | `<literal, offset, size>`                       |
| find_match  | `<literal from, literal to_find, offset, end>`  |
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

    static_assert(find_match<"hello", "ll", 0, 6>() == 2);
    static_assert(starts_with<"hello", "he">());

    std::puts(literal_v<replace_one<"abcdabcd", "bc", "BC">()>);
    // "aBCdabcd"

    std::puts(literal_v<replace_all<"abcdabcd", "bc", "BC">()>);
    // "aBCadBCd"
}
```

## str_name

### nil::xalt::str_name_type

```cpp
int main()
{
    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_name_type_v<YourType>);
    // or string_view
    std::puts(nil::xalt::str_name_type_sv<YourType>.data());
}
```

### nil::xalt::str_name_value

```cpp
int main()
{
    // fixed raw array (const char[N])
    std::puts(nil::xalt::str_name_type_v<100>);
    // or string_view
    std::puts(nil::xalt::str_name_type_sv<100>.data());

    // useful for getting names of enums
}
```

## str_enum

NOTE: Only scans values from 0 to 25. Can be customized to use different start/end/step or a totally different scanning behavior.

```cpp
enum class ABC
{
    A,
    B,
    C
};

int main()
{
    using namespace nil::xalt;

    std::puts(str_enum(ABC::A)); // ABC::A
    std::puts(str_enum(ABC::B)); // ABC::B
    std::puts(str_enum(ABC::C)); // ABC::C

    std::puts(str_enum(ABC(0))); // ABC::A
    std::puts(str_enum(ABC(1))); // ABC::B
    std::puts(str_enum(ABC(2))); // ABC::C
    
    std::puts(str_enum(ABC(5))); // -
}
```

## noisy_type

### nil::xalt::noisy_type

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

## errors

### nil::xalt::undefined

```cpp
int main()
{
    nil::xalt::undefined<int, T>();
    nil::xalt::undefined<1, 2, 3>();
    // will cause compilation error and print out the arguments
}
```

## tlist

### nil::xalt::tlist_types

```cpp
int main()
{
    using namespace nil::xalt;
    using types = tlist_types<int, bool>;
    
    // tlist_types<std::optional<int>, std::optional<bool>>
    using applied = types::apply<std::optional>;

    // std::tuple<std::optional<int>, std::optional<bool>>
    using casted = types::cast<std::tuple>;

    // == tlist_types<int, bool>
    using new_t = to_tlist_types<std::tuple<int, bool>>::type;
    // or
    using new_t = to_tlist_types_t<std::tuple<int, bool>>;
}
```

### nil::xalt::tlist_types

```cpp
constexpr std::size_t twice(std::size_t v) {
    return v * 2;
}

int main()
{
    using namespace nil::xalt;
    using values = tlist_values<0, 1, 2>;

    // tlist_values<0, 2, 4>
    using applied = values::apply<twice>;

    // std::index_sequence<0, 1, 2>
    using casted = values::cast<std::index_sequence>;

    // WIP (does not work yet due to template deduction)
    // index_sequence is an alias to integer_sequence<size_t>
    // so instantiation fails
    //
    // == tlist_values<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>
    using new_v = to_tlist_values<std::make_index_sequence<10>>::type;
    // or
    using new_v = to_tlist_values_t<std::make_index_sequence<10>>;
}
```

## cast

### nil::xalt::explicit_cast

```cpp
int main()
{
    int v = 100;

    nil::xalt::explicit_cast<int&> e_l(&v);
    e.cast(); // to int&
    
    nil::xalt::explicit_cast<int&&> e_r(&v);
    e.cast(); // to int&&

    // useful in template programming where
    // the type has to be stored and re-casted
}
```

### nil::xalt::implicit_cast (WIP)
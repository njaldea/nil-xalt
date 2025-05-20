# nil/xalt

A grab-bag of small utilities that don’t fit into any specific module.

## Table of Contents

- [checks](#checks)
    - [nil::xalt::is_of_template](#nilxaltis_of_template)
- [literal](#literal)
    - [nil::xalt::literal](#nilxaltliteral)
    - [operations](#operations)
- [tlist](#tlist)
    - [nil::xalt::tlist_types](#nilxalttlist_types)
    - [nil::xalt::tlist_values](#nilxalttlist_values)
- [str_name](#str_name)
    - [nil::xalt::str_name_type](#nilxaltstr_name_type)
    - [nil::xalt::str_name_value](#nilxaltstr_name_value)
- [str_enum](#str_enum)
    - [nil::xalt::str_enum](#nilxaltstr_enum)
    - [nil::xalt::str_enum_start](#nilxaltstr_enum_start)
    - [nil::xalt::str_enum_values](#nilxaltstr_enum_values)
- [noisy_type](#noisy_type)
    - [nil::xalt::noisy_type](#nilxaltnoisy_type)
- [errors](#errors)
    - [nil::xalt::undefined](#nilxaltundefined)
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
| find_first  | `<literal from, literal to_find, offset, end>`  |
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

    static_assert(find_first<"hello", "ll", 0, 6>() == 2);
    static_assert(starts_with<"hello", "he">());

    std::puts(literal_v<replace_one<"abcdabcd", "bc", "BC">()>);
    // "aBCdabcd"

    std::puts(literal_v<replace_all<"abcdabcd", "bc", "BC">()>);
    // "aBCadBCd"
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

    using type_at_0 = types::at<0>; // int
    using type_at_1 = types::at<1>; // bool
}
```

### nil::xalt::tlist_values

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

    // tlist_values<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>
    using new_v = to_tlist_values<std::make_index_sequence<10>>::type;
    // or
    using new_v = to_tlist_values_t<std::make_index_sequence<10>>;

    static constexpr auto value_at_0 = values::at<0>; // 0
    static constexpr auto value_at_1 = values::at<1>; // 1
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

| Trait                     | Purpose                                 | Example                          |
|---------------------------|-----------------------------------------|----------------------------------|
| `str_enum_start<T>`       | Customize the first enum to inspect     | `ABC::A`                         |
| `str_enum_values<T>`      | Override enum value scanning entirely   | `tlist_values<ABC::A, ABC::B>`   |
| `str_enum(T)`             | Get string name or `"-"` fallback       | `"ABC::B"`                       |

### nil::xalt::str_enum

This method converts an enum to their respective `std::string_view` name format.

```cpp
enum class ABC
{
    A = 0,
    B,
    C
};

int main()
{
    using namespace nil::xalt;

    std::puts(str_enum(ABC::A).data()); // ABC::A
    std::puts(str_enum(ABC::B).data()); // ABC::B
    std::puts(str_enum(ABC::C).data()); // ABC::C

    std::puts(str_enum(ABC(0)).data()); // ABC::A
    std::puts(str_enum(ABC(1)).data()); // ABC::B
    std::puts(str_enum(ABC(2)).data()); // ABC::C
    
    // Returns "-" if the enum value does not correspond to any known enumerator
    std::puts(str_enum(ABC(5)).data()); // -
}
```

This works by doing the following:
- start from the first enum value
- if the value is valid, add it to the list
- if the value is not valid, *stop the search*
- get the next candidate then repeat the process

The next candidate is decided depending on the type of enum.
There are two types of enums, *counting* and *masking*.

*counting* enums are those that has values increment by one.
*masking* enums are typically used as bitmasks and expected to have power-of-two values (e.g., 1, 2, 4, 8, …).
*masking* enum considers 0 as an invalid value.

If the value can be converted into the target enum type and matches a known enumerator name, it is considered valid.

### nil::xalt::str_enum_start

To override the first enum value, specialize the following:

```cpp
template <>
struct nil::xalt::str_enum_start<ABC>
{
    static constexpr auto value = ABC::A;
};
```

By default, the enum iteration starts at `0` if a valid enum value exists at `0`; otherwise, it starts at `1`.

The next value is assumed to be either:
 - ` + 1` from the current value if a counting enum
 - `<< 1` from the current value if a masking enum
    - a masking enum does not have a `0` value

The search ends when the next value is not a valid enum value.

### nil::xalt::str_enum_values

To override the searching logic entirely, specialize the following:

```cpp
template <>
struct nil::xalt::str_enum_values<ABC>
{
    using type = nil::xalt::tlist_values<ABC::A, ABC::B, ABC::C>;
};
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
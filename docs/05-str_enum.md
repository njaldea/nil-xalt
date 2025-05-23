# str_enum

Contains utility templates to stringify enums.

| Trait                     | Purpose                                 | Example                                     |
|---------------------------|-----------------------------------------|---------------------------------------------|
| `str_enum_start<T>`       | Customize the first enum to inspect     | `ABC::A`                                    |
| `str_enum_values<T>`      | Override enum value scanning entirely   | `tlist<typify<ABC::A>, typify<ABC::B>>`     |
| `str_enum(T)`             | Get string name or `"-"` fallback       | `"ABC::B"`                                  |

## nil::xalt::str_enum

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
- if the value is not valid, *terminate the scan*
- get the next candidate then repeat the process

The next candidate is decided depending on the type of enum.
There are two types of enums, *counting* and *masking*.

*counting* enums are those that have values increment by one.
*masking* enums are typically used as bitmasks and expected to have power-of-two values (e.g., 1, 2, 4, 8, …).
*masking* enum considers 0 as an invalid value.

If the value can be converted into the target enum type and matches a known enumerator name, it is considered valid.

## nil::xalt::str_enum_start

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

## nil::xalt::str_enum_values

To override the searching logic entirely, specialize the following:

```cpp
template <>
struct nil::xalt::str_enum_values<ABC>
{
    using type = nil::xalt::tlist<
        nil::xalt::typify<ABC::A>,
        nil::xalt::typify<ABC::B>,
        nil::xalt::typify<ABC::C>
    >;
};
```

## Dependencies

- [typed](./09-typed.md)
- [tlist](./03-tlist.md)
- [literal](./02-literal.md)
- [str_name](./04-str_name.md)

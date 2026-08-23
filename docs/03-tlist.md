# tlist

A compile-time type list for storing and transforming a sequence of types.

Includes: `#include <nil/xalt/tlist.hpp>`

## Overview

`nil::xalt::tlist` is a template metaprogramming utility that provides:
- Type sequence manipulation
- Type transformation
- Type querying and filtering

## Interface

The `tlist` class provides the following categories of operations:

### API Table

| Member Alias | Description |
|---|---|
| `cast<U>` | Rebind contained types into `U<T...>`. |
| `cast_t<U>` | Rebind then read `::type` from the result. |
| `apply<U, C...>` | Map each type through `U<T, C...>`. `U` must take at least one type parameter. |
| `apply_t<U, C...>` | Map each type through `typename U<T, C...>::type`. `U` must take at least one type parameter. |
| `any_of<P, C...>` | Check whether any type satisfies `P<T, C...>::value`. |
| `all_of<P, C...>` | Check whether every type satisfies `P<T, C...>::value`. |
| `contains<U>` | Check whether `U` occurs in the list. |
| `at<I>` | Type at index `I` (bounds-checked). |

The following free utilities operate on `tlist` types:

| Utility | Description |
|---|---|
| `tlist_remove_if_t<I, P, C...>` | Filter out types where `P<T, C...>::value` is `true`. The output list defaults to empty internally. |
| `tlist_join_t<L...>` | Concatenate `tlist<...>` types. |
| `tlist_dedupe_t<L>` | Remove duplicate types, preserving first occurrence order. |

### Type Transformations
- `cast` - Convert the type list to another template (e.g., `std::tuple`)
- `cast_t` - Same as `cast` but gets the nested `::type`
- `apply` - Apply a template to each type in the list
- `apply_t` - Apply a template and get its nested `::type` for each type

```cpp
// Convert to std::tuple
using as_tuple = my_types::cast<std::tuple>;        // std::tuple<int, bool, float>

// Apply template to each type
using ptrs = my_types::apply<std::add_pointer_t>;   // tlist<int*, bool*, float*>

// Apply a trait and extract nested ::type
using ptrs2 = my_types::apply_t<std::add_pointer>;  // tlist<int*, bool*, float*>
```

### Properties
- `size` - Number of types in the list
- `contains<U>` - Check if the list contains type `U`
- `at<I>` - Access type at index I (bounds-checked)

```cpp
using my_types = tlist<int, bool, float>;
static_assert(my_types::size == 3);
static_assert(my_types::contains<float>);
using first_type = my_types::at<0>;  // int
```

### Operations
- `any_of` - Check if any type satisfies a predicate (false when empty)
- `all_of` - Check if all types satisfy a predicate (true when empty)
- `contains` - Check whether a type occurs in the list
- `at` - Access a type by index

Filtering, joining, and deduplication are provided by the free utilities
`tlist_remove_if_t`, `tlist_join_t`, and `tlist_dedupe_t` described above.

```cpp
using my_types = tlist<int, float, bool>;

// Check for floating point types
static_assert(my_types::any_of<std::is_floating_point>);
static_assert(!my_types::all_of<std::is_floating_point>);

// Remove floating point types with the free utility
template <typename T>
struct is_floating_point { static constexpr bool value = std::is_floating_point_v<T>; };

using non_floats = tlist_remove_if_t<my_types, is_floating_point>;  // tlist<int, bool>

// Join lists with the free utility
using more = tlist<char, long>;
using all = tlist_join_t<my_types, more>; // tlist<int, float, bool, char, long>

// Deduplicate by first appearance with the free utility
using with_dups = tlist<int, bool, int, float, bool>;
using dedup = tlist_dedupe_t<with_dups>; // tlist<int, bool, float>

// Predicates may take extra parameters via the C... pack
static_assert(my_types::any_of<std::is_same, int>);
using without_float = tlist_remove_if_t<my_types, std::is_same, float>; // tlist<int, bool>
```

Behavior notes:
- `tlist_remove_if_t` preserves the original order of retained types.
- `tlist_join_t` expects `tlist<...>` arguments (not arbitrary templates).
- `at<I>` is bounds-checked and currently implemented recursively.
- `apply` expects a template that yields a type directly; `apply_t` expects a template with nested `::type`.
- `tlist_dedupe_t` preserves order and keeps the first instance of each type.

### Type Conversion
Helper types for converting other templates to `tlist`:
- `typify` - Convert values to types (allows storing of non-type template parameters)
- `to_tlist` - Convert template instances to `tlist`
- `to_tlist_t` - Convenience alias for `typename to_tlist<T>::type`

```cpp
// Convert from other templates
using from_tuple = to_tlist_t<std::tuple<int, float, bool>>;  // tlist<int, float, bool>
using from_seq = to_tlist_t<std::index_sequence<1, 2, 3>>;    // tlist<typify<1>, typify<2>, typify<3>>

// std::make_index_sequence<N> is an integer_sequence, so it is converted too
using indices = to_tlist_t<std::make_index_sequence<3>>;     // tlist<typify<0>, typify<1>, typify<2>>
```

## Predicates

A predicate must be a template that provides `static constexpr bool value`:

```cpp
template <typename T, typename U>
struct is_same {
    static constexpr bool value = std::is_same_v<T, U>;
};

// Many standard type traits (e.g., std::is_same) can be used directly
// since they already expose a ::value member.
```

## Dependencies

- [typed](./10-typed.md)
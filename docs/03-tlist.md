# tlist

A compile-time type list for storing and transforming a sequence of types.

## Overview

`nil::xalt::tlist` is a template metaprogramming utility that provides:
- Type sequence manipulation
- Type transformation
- Type querying and filtering

## Interface

The `tlist` class provides the following categories of operations:

### Type Transformations
- `cast` - Convert the type list to another template (e.g., `std::tuple`)
- `cast_t` - Same as `cast` but gets the nested `::type`
- `apply` - Apply a template to each type in the list
- `apply_t` - Apply a template and get its nested `::type` for each type

```cpp
// Convert to std::tuple
using as_tuple = my_types::cast<std::tuple>;        // std::tuple<int, bool, float>

// Apply template to each type
using ptrs = my_types::apply<std::add_pointer_>;    // tlist<int*, bool*, float*>
// or
using ptrs = my_types::apply_t<std::add_pointer>;   // tlist<int*, bool*, float*>
```

### Properties
- `size` - Number of types in the list
- `at<I>` - Access type at index I (bounds-checked)

```cpp
using my_types = tlist<int, bool, float>;
static_assert(my_types::size == 3);
using first_type = my_types::at<0>;  // int
```

### Operations
- `any_of` - Check if any type satisfies a predicate (false when empty)
- `all_of` - Check if all types satisfy a predicate (true when empty)
- `remove_if` - Create new list without types matching predicate

```cpp
using my_types = tlist<int, float, bool>;

// Check if any type is integral
// `std::is_floating_point` has `:value`
static_assert(from_tuple::any_of<std::is_floating_point>);
static_assert(!from_tuple::all_of<std::is_floating_point>);

// Remove non-integral types
using integers = my_types::remove_if<is_floating_point>;        // tlist<int, bool>
```

### Type Conversion
Helper types for converting other templates to `tlist`:
- `typify` - Convert values to types (allows storing of non-type template parameters)
- `to_tlist` - Convert template instances to `tlist`
- `to_tlist_t` - Convenience alias for `typename to_tlist<T>::type`

```cpp
// Convert from other templates
using from_tuple = to_tlist_t<std::tuple<int, float, bool>>;  // tlist<int, float, bool>
using from_seq = to_tlist_t<std::index_sequence<1,2,3>>;      // tlist<typify<1>, typify<2>, typify<3>>
```

## Predicates

A predicate must be a template that provides `static constexpr bool value`:

```cpp
template <typename T, typename U>
struct is_same {
    static constexpr bool value = std::is_same_v<T, U>;
};
```

## Dependencies

- [typed](./09-typed.md)
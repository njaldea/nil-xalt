# tlist_types

A compile-time type list for storing and transforming a sequence of types

## nil::xalt::tlist_types

```cpp
template <typename T, typename U>
struct is_same // this is a predicate
{
    static constexpr bool value = std::is_same_v<T, U>;
};

int main()
{
    using namespace nil::xalt;
    
    // tlist_types<std::optional<int>, std::optional<bool>>
    using applied = tlist_types<int, bool>::apply<std::optional>;

    // == std::tuple<std::optional<int>, std::optional<bool>>
    using casted = tlist_types<int, bool>::cast<std::tuple>;

    // == tlist_types<int, bool>
    using new_t = to_tlist_types<std::tuple<int, bool>>::type;
    // or
    using new_t = to_tlist_types_t<std::tuple<int, bool>>;

    using type_at_0 = tlist_types<int, bool>::at<0>; // int
    using type_at_1 = tlist_types<int, bool>::at<1>; // bool

    // 1st argument is a predicate
    static_assert(tlist_types<int, bool>::any_of<is_same, bool>);
    static_assert(!tlist_types<int, bool>::any_of<is_same, float>);
    // also supports all_of

    // == tlist_types<int>
    using filtered = tlist_types<int, bool>::remove_if<is_same, bool>;
}
```

### predicate

A struct with `static constexpr bool value`. This is used by `tlist_types` for the following:
- `any_of`
- `all_of`
- `remove_if`

The first argument is the template parameter of tlist_types.
The rest passed to the operation are forwarded to the predicate.

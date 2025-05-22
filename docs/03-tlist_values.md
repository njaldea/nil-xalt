# tlist_values

Contains utility templates to hold multiple non-type template parameters and interact with them.

## nil::xalt::tlist_values

```cpp
struct multiply
{
    constexpr auto operator()(auto l, auto r) {
        return l * r;
    }
};

struct is_equal
{
    constexpr auto operator()(auto l, auto r) {
        return l == r;
    }
};

int main()
{
    using namespace nil::xalt;
    using values = tlist_values<0, 1, 2>;

    // tlist_values<0, 2, 4>
    // first argument is a predicate
    using applied = values::apply<multiply, 2>;

    // std::index_sequence<0, 1, 2>
    using casted = values::cast<std::index_sequence>;

    // tlist_values<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>
    using new_v = to_tlist_values<std::make_index_sequence<10>>::type;
    // or
    using new_v = to_tlist_values_t<std::make_index_sequence<10>>;

    static constexpr auto value_at_0 = values::at<0>; // 0
    static constexpr auto value_at_1 = values::at<1>; // 1
    
    // first argument is a predicate
    static_assert(type::any_of<is_equal, 1>);
    static_assert(!type::any_of<is_equal, 2>);
    // also supports all_of

    // == tlist_values<0, 2>
    using filtered = type::remove_if<is_equal, 1>;
}
```

### predicate

A struct with call operator. This is used by `tlist_values` for the following:
- `apply`
- `any_of`
- `all_of`
- `remove_if`

The first argument is the non-type template paramter of tlist_values.
The rest passed to the operation are forwarded to the predicate.

# checks

Contains utility templates for checking truthy-ness

## nil::xalt::is_of_template

```cpp
int main()
{
    using Tup = std::tuple<int, float>;
    struct S {};

    // Works with the trait
    static_assert(
        nil::xalt::is_of_template<
            Tup,        // your type
            std::tuple  // any typename <typename...>
        >::value
    );

    // And with the concept-style alias
    static_assert(nil::xalt::is_of_template_v<Tup, std::tuple>);
    static_assert(!nil::xalt::is_of_template_v<S, std::tuple>);
};
```

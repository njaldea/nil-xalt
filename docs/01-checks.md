# checks

Contains utility templates for checking truthy-ness

## nil::xalt::is_of_template

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

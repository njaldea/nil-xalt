# typed

Contains utility templates containing unique identifier for a type.

## nil::xalt::typify

Wraps a non-type template parameter.

```cpp
// Store integers in the type system
using one = nil::xalt::typify<1>;
using two = nil::xalt::typify<2>;

// Use with standard containers
using int_list = std::tuple<typify<1>, typify<2>, typify<3>>;

// Convert sequences to type lists
using seq = std::make_index_sequence<3>;             // std::index_sequence<0,1,2>
using typed_seq = nil::xalt::to_tlist_t<seq>;        // tlist<typify<0>, typify<1>, typify<2>>

// Access the value
static_assert(typify<42>::value == 42);                   // Using function call syntax
static_assert(static_cast<int>(typify<42>::value) == 42); // Using conversion operator
```

## nil::xalt::type_id

```cpp
int main()
{
    constexpr const void* id = nil::xalt::type_id<int>;
}
```

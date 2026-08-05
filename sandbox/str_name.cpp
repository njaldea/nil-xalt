#include <cstdint>
#include <nil/xalt/str_name.hpp>
#include <nil/xalt/typed.hpp>

#include <iostream>

template <typename T>
void print_type_variants()
{
    constexpr auto name = nil::xalt::str_name<T>();
    constexpr auto short_name = nil::xalt::str_short_name<T>();
    constexpr auto scope_name = nil::xalt::str_scope_name<T>();
    constexpr auto short_base_name = nil::xalt::str_short_base_name<T>();

    std::cout << "str_name            : " << nil::xalt::literal_sv<name> << '\n';
    std::cout << "str_short_name      : " << nil::xalt::literal_sv<short_name> << '\n';
    std::cout << "str_scope_name      : " << nil::xalt::literal_sv<scope_name> << '\n';
    std::cout << "str_short_base_name : " << nil::xalt::literal_sv<short_base_name> << '\n';
}

namespace sandbox
{
    enum class Zip : std::uint8_t
    {
        ABC,
        DEF,
        GHI,
        JKL
    };

    struct Person
    {
    };

    template <typename T>
    struct Box
    {
        template <typename U>
        struct InnerBox
        {
        };
    };
} // namespace

int main()
{
    using namespace nil::xalt;
    using namespace sandbox;

    std::cout << "=== type variants: Person ===\n";
    print_type_variants<Person>();

    std::cout << "\n=== type variants: Box<Person> ===\n";
    print_type_variants<Box<Person>>();

    std::cout << "\n=== type variants: Box<Box<Person>> ===\n";
    print_type_variants<Box<Box<Person>>>();

    std::cout << "\n=== type variants: Box<Box<Box<int>>> ===\n";
    print_type_variants<Box<Box<Box<int>>>>();

    std::cout << "\n=== type variants: Box<Person>::InnerBox<Person> ===\n";
    print_type_variants<Box<Person>::InnerBox<Person>>();

    std::cout << "\n=== type variants: Box<Person>::InnerBox<Box<Person>> ===\n";
    print_type_variants<Box<Person>::InnerBox<Box<Person>>>();

    std::cout << "\n=== typify value variants ===\n";
    {
        constexpr auto enum_name = str_name<typify<Zip::DEF>>();
        constexpr auto enum_short_name = str_short_name<typify<Zip::DEF>>();
        constexpr auto enum_scope_name = str_scope_name<typify<Zip::DEF>>();
        constexpr auto cast_name = str_name<typify<Zip(100)>>();

        std::cout << "str_name<typify<Zip::DEF>>: " << literal_sv<enum_name> << '\n';
        std::cout << "str_short_name<typify<Zip::DEF>>: " << literal_sv<enum_short_name> << '\n';
        std::cout << "str_scope_name<typify<Zip::DEF>>: " << literal_sv<enum_scope_name> << '\n';
        std::cout << "str_name<typify<Zip(100)>>: " << literal_sv<cast_name> << '\n';
    }

    return 0;
}

#include <cstdint>
#include <nil/xalt/errors.hpp>
#include <nil/xalt/fn_make.hpp>
#include <nil/xalt/fn_sign.hpp>
#include <nil/xalt/literal.hpp>
#include <nil/xalt/noisy_type.hpp>
#include <nil/xalt/str_enum.hpp>
#include <nil/xalt/str_name.hpp>
#include <nil/xalt/transparent_stl.hpp>
#include <nil/xalt/typed.hpp>

#include <functional>
#include <iostream>
#include <string_view>

template <nil::xalt::literal T>
struct MyType
{
    std::string_view get_value()
    {
        return nil::xalt::literal_sv<T>;
    }
};

enum class Zip : std::uint8_t
{
    ABC,
    DEF,
    GHI,
    JKL
};

void foo()
{
    std::cout << __FILE__ << ':' << __LINE__ << ':' << (const char*)(__FUNCTION__) << std::endl;
}

void foo_n() noexcept;

struct bar
{
    void foo()
    {
        (void)this;
        std::cout << __FILE__ << ':' << __LINE__ << ':' << (const char*)(__PRETTY_FUNCTION__)
                  << std::endl;
    }

    void foo_c() const;
    void foo_n() noexcept;
    void foo_n_c() const noexcept;
};

template <bool nc, typename T>
void check(T /* t */)
{
    using nil::xalt::fn_sign;
    static_assert(std::is_same_v<typename fn_sign<T>::return_type, void>);
    static_assert(std::is_same_v<typename fn_sign<T>::class_type, void>);
    static_assert(fn_sign<T>::is_const);
    static_assert(fn_sign<T>::is_noexcept == nc);
}

template <bool nc, bool cc, typename T>
void bar_check(T /* t */)
{
    using nil::xalt::fn_sign;
    static_assert(std::is_same_v<typename fn_sign<T>::return_type, void>);
    static_assert(std::is_same_v<typename fn_sign<T>::class_type, bar>);
    static_assert(fn_sign<T>::is_const == cc);
    static_assert(fn_sign<T>::is_noexcept == nc);
}

template <auto T>
struct Woof
{
    void wtf(bar& b)
    {
        static_assert(std::is_same_v<typename nil::xalt::fn_sign<decltype(T)>::class_type, bar>);
        std::invoke(T, b);
    }

    void wtf()
    {
        static_assert(std::is_same_v<typename nil::xalt::fn_sign<decltype(T)>::class_type, void>);
        std::invoke(T);
    }
};

int main()
{
    using namespace nil::xalt;

    using type = MyType<concat<"Hello", " ", "World", " ", "wtf">()>;
    constexpr auto z = substr<"abcdefghijklmnop", 4, 5>();
    std::cout << literal_sv<z> << std::endl;
    std::cout << find_first<"abcdefghi", "def">() << std::endl;
    std::cout << literal_sv<replace_one<"abcdefghi", "def", "_njla_">()> << std::endl;
    std::cout << literal_sv<replace_all<"abcdefghiabcdefghi", "def", "_njla_">()> << std::endl;
    std::cout << type().get_value() << std::endl;
    std::cout << str_name_sv<type> << std::endl;
    std::cout << str_name_sv<typify<literal("asd")>> << std::endl;
    std::cout << str_name_sv<typify<Zip::ABC>> << std::endl;
    std::cout << str_name_sv<typify<Zip(100)>> << std::endl;
    std::cout << nil::xalt::str_enum(Zip::DEF) << std::endl;

    check<false>(&foo);
    check<true>(&foo_n);
    bar_check<false, false>(&bar::foo);
    bar_check<false, true>(&bar::foo_c);
    bar_check<true, false>(&bar::foo_n);
    bar_check<true, true>(&bar::foo_n_c);

    {
        Woof<&bar::foo> w;
        bar b;
        w.wtf(b);
    }

    {
        Woof<&foo> w;
        w.wtf();
    }

    {
        using namespace nil::xalt;
        using namespace nil::xalt::detail;
        using B = nil::xalt::noisy_type<"B">;

        using A = nil::xalt::noisy_type<"A", B&, const B&>;

        auto b = B();
        const auto& [a, ua, sa] = std::make_tuple( //
            nil::xalt::fn_make<A>("dsd", 2, "std", 1, b, "asd", B(), b, B()),
            nil::xalt::fn_make_unique<A>(2, "std", 1, b, B(), "asd", "dsad", "hello"),
            nil::xalt::fn_make_shared<A>(2, "std", 1, b, B(), "asd", "dsad", "hello")
        );
    }
    return 0;
}

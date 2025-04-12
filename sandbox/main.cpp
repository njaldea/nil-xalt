#include <nil/xalt.hpp>
#include <nil/xalt/fn_make.hpp>
#include <nil/xalt/fn_sign.hpp>
#include <nil/xalt/literal.hpp>
#include <nil/xalt/str_name.hpp>
#include <nil/xalt/stringify.hpp>
#include <nil/xalt/type_id.hpp>

#include <functional>
#include <iostream>
#include <string_view>

template <nil::xalt::tp_literal T>
struct MyType
{
    std::string_view get_value()
    {
        return nil::xalt::literal<T>::value;
    }
};

enum class Zip
{
    ABC,
    DEF,
    GHI
};

auto stringify(Zip zip)
{
    return nil::xalt::stringify<Zip::ABC, Zip::DEF, Zip::GHI>(zip);
}

void foo()
{
    std::cout << __FILE__ << ':' << __LINE__ << ':' << (const char*)(__FUNCTION__) << std::endl;
}

void foo_n() noexcept;

struct bar
{
    void foo()
    {
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
    static_assert(std::is_same_v<typename fn_sign<T>::class_name, void>);
    static_assert(fn_sign<T>::is_const);
    static_assert(fn_sign<T>::is_noexcept == nc);
}

template <bool nc, bool cc, typename T>
void bar_check(T /* t */)
{
    using nil::xalt::fn_sign;
    static_assert(std::is_same_v<typename fn_sign<T>::return_type, void>);
    static_assert(std::is_same_v<typename fn_sign<T>::class_name, bar>);
    static_assert(fn_sign<T>::is_const == cc);
    static_assert(fn_sign<T>::is_noexcept == nc);
}

template <auto T>
struct Woof
{
    void wtf(bar& b)
    {
        static_assert(std::is_same_v<typename nil::xalt::fn_sign<decltype(T)>::class_name, bar>);
        std::invoke(T, b);
    }

    void wtf()
    {
        static_assert(std::is_same_v<typename nil::xalt::fn_sign<decltype(T)>::class_name, void>);
        std::invoke(T);
    }
};

#define PR                                                                                         \
    std::cout << __FILE__ << ':' << __LINE__ << ':' << (const char*)(__PRETTY_FUNCTION__)          \
              << std::endl

struct B
{
    B()
    {
        PR;
    }

    B(const B&)
    {
        PR;
    }

    B(B&&) noexcept
    {
        PR;
    }

    B& operator=(const B& /* b */)
    {
        PR;
        return *this;
    }

    B& operator=(B&& /* b */) noexcept
    {
        PR;
        return *this;
    }

    ~B() = default;
};

template <typename... T>
struct A
{
    explicit A(T... /* t */)
    {
        PR;
    }
};

int main()
{
    using nil::xalt::str_name_type;
    using nil::xalt::str_name_value;
    using nil::xalt::tp_literal;
    using nil::xalt::type_id;

    using type = MyType<nil::xalt::concat("Hello", " ", "World", " ", "wtf")>;
    std::cout << type().get_value() << std::endl;
    std::cout << str_name_type<type>::value << std::endl;
    std::cout << str_name_value<tp_literal("asd")>::value << std::endl;
    std::cout << str_name_value<Zip::ABC>::value << std::endl;
    std::cout << stringify(Zip::DEF) << std::endl;

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
        static_assert(0b00000 == pow_2(0));
        static_assert(0b00001 == pow_2(1));
        static_assert(0b00010 == pow_2(2));
        static_assert(0b00100 == pow_2(3));
        static_assert(0b01000 == pow_2(4));
        static_assert(0b10000 == pow_2(5));
        static_assert(0b00000 == make_mask(0));
        static_assert(0b00001 == make_mask(1));
        static_assert(0b00011 == make_mask(2));
        static_assert(0b00111 == make_mask(3));
        static_assert(0b01111 == make_mask(4));
        static_assert(0b11111 == make_mask(5));
        using AA = A<B&, const B&>;
        auto b = B();
        auto [a, ua, sa] = std::make_tuple( //
            nil::xalt::fn_make<AA>(2, "std", 1, b, B(), "asd", "dsad", "hello"),
            nil::xalt::fn_make_unique<AA>(2, "std", 1, b, B(), "asd", "dsad", "hello"),
            nil::xalt::fn_make_shared<AA>(2, "std", 1, b, B(), "asd", "dsad", "hello")
        );

        return 0;
    }
}

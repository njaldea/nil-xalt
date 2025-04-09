#include "nil/xalt/literal.hpp"
#include <nil/xalt.hpp>
#include <nil/xalt/enum_name.hpp>
#include <nil/xalt/fsign.hpp>
#include <nil/xalt/type_id.hpp>
#include <nil/xalt/type_name.hpp>
#include <nil/xalt/value_name.hpp>

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

auto enum_name(Zip zip)
{
    return nil::xalt::enum_name<Zip, Zip::ABC, Zip::DEF, Zip::GHI>(zip);
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
        std::cout << __FILE__ << ':' << __LINE__ << ':' << (const char*)(__FUNCTION__) << std::endl;
    }

    void foo_c() const;
    void foo_n() noexcept;
    void foo_n_c() const noexcept;
};

template <bool nc, typename T>
void check(T /* t */)
{
    using nil::xalt::fsign;
    static_assert(std::is_same_v<typename fsign<T>::return_type, void>);
    static_assert(std::is_same_v<typename fsign<T>::class_name, void>);
    static_assert(fsign<T>::is_const);
    static_assert(fsign<T>::is_noexcept == nc);
}

template <bool nc, bool cc, typename T>
void bar_check(T /* t */)
{
    using nil::xalt::fsign;
    static_assert(std::is_same_v<typename fsign<T>::return_type, void>);
    static_assert(std::is_same_v<typename fsign<T>::class_name, bar>);
    static_assert(fsign<T>::is_const == cc);
    static_assert(fsign<T>::is_noexcept == nc);
}

template <auto T>
struct Woof
{
    void wtf(bar& b)
    {
        static_assert(std::is_same_v<typename nil::xalt::fsign<decltype(T)>::class_name, bar>);
        std::invoke(T, b);
    }

    void wtf()
    {
        static_assert(std::is_same_v<typename nil::xalt::fsign<decltype(T)>::class_name, void>);
        std::invoke(T);
    }
};

int main()
{
    using nil::xalt::tp_literal;
    using nil::xalt::type_id;
    using nil::xalt::type_name;
    using nil::xalt::value_name;

    using type = MyType<nil::xalt::concat("Hello", " ", "World", " ", "wtf")>;
    std::cout << type().get_value() << std::endl;
    std::cout << type_name<type>::value << std::endl;
    std::cout << value_name<tp_literal("asd")>::value << std::endl;
    std::cout << value_name<Zip::ABC>::value << std::endl;
    std::cout << enum_name(Zip::DEF) << std::endl;

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
}

#include <nil/xalt/str_name.hpp>
#include <nil/xalt/typed.hpp>

#include <gtest/gtest.h>

using namespace nil::xalt;

namespace aa::bb
{
    template <typename...>
    class Hello;

    enum class ETest
    {
        A = 0,
        B,
        C
    };
}

TEST(str_name, type)
{
    using t = aa::bb::Hello<bool, int>;
    ASSERT_EQ(str_name_sv<t>, "aa::bb::Hello<bool, int>");
}

TEST(str_name, value)
{
    ASSERT_EQ(str_name_sv<typify<aa::bb::ETest::A>>, "aa::bb::ETest::A");
    ASSERT_EQ(str_name_sv<typify<aa::bb::ETest::B>>, "aa::bb::ETest::B");
    ASSERT_EQ(str_name_sv<typify<aa::bb::ETest::C>>, "aa::bb::ETest::C");
    ASSERT_EQ(str_name_sv<typify<aa::bb::ETest(4)>>, "(aa::bb::ETest)4");
}

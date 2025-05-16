#include <nil/xalt/str_name.hpp>

#include <gtest/gtest.h>

using namespace nil::xalt;

namespace aa::bb
{
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
    ASSERT_EQ(str_name_type_sv<aa::bb::Hello>, "aa::bb::Hello");
}

TEST(str_name, value)
{
    ASSERT_EQ(str_name_value_sv<aa::bb::ETest::A>, "aa::bb::ETest::A");
    ASSERT_EQ(str_name_value_sv<aa::bb::ETest::B>, "aa::bb::ETest::B");
    ASSERT_EQ(str_name_value_sv<aa::bb::ETest::C>, "aa::bb::ETest::C");
    ASSERT_EQ(str_name_value_sv<aa::bb::ETest(4)>, "(aa::bb::ETest)4");
}

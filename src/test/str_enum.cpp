#include <nil/xalt/str_enum.hpp>

#include <gtest/gtest.h>

using namespace nil::xalt;

enum class ETest
{
    A = 0,
    B = 1,
    C = 2,
    D = 4 // not listed since there is a gap
};

TEST(str_enum, increment)
{
    using actual_values = str_enum_values_t<ETest>;
    using expected_values = tlist<typify<ETest::A>, typify<ETest::B>, typify<ETest::C>>;
    static_assert((std::is_same_v<actual_values, expected_values>));

    ASSERT_EQ(str_enum(ETest::A), "ETest::A");
    ASSERT_EQ(str_enum(ETest::B), "ETest::B");
    ASSERT_EQ(str_enum(ETest::C), "ETest::C");
    ASSERT_EQ(str_enum(ETest(4)), "-");
}

enum class EMaskedTest
{
    A = 0b1 << 0, // 1
    B = 0b1 << 1, // 2
    C = 0b1 << 2, // 4
    D = 0b1 << 4  // 16 not listed since there is a gap
};

TEST(str_enum, mask)
{
    using actual_values = str_enum_values_t<EMaskedTest>;
    using expected_values
        = tlist<typify<EMaskedTest::A>, typify<EMaskedTest::B>, typify<EMaskedTest::C>>;
    static_assert((std::is_same_v<actual_values, expected_values>));

    ASSERT_EQ(str_enum(EMaskedTest::A), "EMaskedTest::A");
    ASSERT_EQ(str_enum(EMaskedTest::B), "EMaskedTest::B");
    ASSERT_EQ(str_enum(EMaskedTest::C), "EMaskedTest::C");
    ASSERT_EQ(str_enum(EMaskedTest(0b1 << 3)), "-");
}

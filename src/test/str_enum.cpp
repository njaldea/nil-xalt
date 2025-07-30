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

enum class EMaskedTest1
{
    A1 = 0b1 << 0, // 1
    B1 = 0b1 << 1, // 2
    C1 = 0b1 << 2, // 4
    D1 = 0b1 << 4  // 16 not listed since there is a gap
};

enum EMaskedTest2
{
    A2 = 0b1 << 0, // 1
    B2 = 0b1 << 1, // 2
    C2 = 0b1 << 2, // 4
    D2 = 0b1 << 4  // 16 not listed since there is a gap
};

TEST(str_enum, mask_enum_class)
{
    using actual_values = str_enum_values_t<EMaskedTest1>;
    using expected_values
        = tlist<typify<EMaskedTest1::A1>, typify<EMaskedTest1::B1>, typify<EMaskedTest1::C1>>;
    static_assert((std::is_same_v<actual_values, expected_values>));

    ASSERT_EQ(str_enum(EMaskedTest1::A1), "EMaskedTest1::A1");
    ASSERT_EQ(str_enum(EMaskedTest1::B1), "EMaskedTest1::B1");
    ASSERT_EQ(str_enum(EMaskedTest1::C1), "EMaskedTest1::C1");
    ASSERT_EQ(str_enum(EMaskedTest1(0b1 << 3)), "-");
}

TEST(str_enum, mask_enum)
{
    using actual_values = str_enum_values_t<EMaskedTest2>;
    using expected_values
        = tlist<typify<EMaskedTest2::A2>, typify<EMaskedTest2::B2>, typify<EMaskedTest2::C2>>;
    static_assert((std::is_same_v<actual_values, expected_values>));

    ASSERT_EQ(str_enum(EMaskedTest2::A2), "A2");
    ASSERT_EQ(str_enum(EMaskedTest2::B2), "B2");
    ASSERT_EQ(str_enum(EMaskedTest2::C2), "C2");
    ASSERT_EQ(str_enum(EMaskedTest2(0b1 << 3)), "-");
}

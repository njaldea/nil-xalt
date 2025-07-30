#include <nil/xalt/str_name.hpp>
#include <nil/xalt/typed.hpp>

#include <gtest/gtest.h>

using namespace nil::xalt;

namespace aa::bb
{
    template <typename...>
    class Hello1;

    template <typename...>
    struct Hello2
    {
        template <typename...>
        struct World2
        {
        };
    };

    enum class ETest1
    {
        A1 = 0,
        B1,
        C1
    };

    enum ETest2
    {
        A2 = 0,
        B2,
        C2
    };
}

TEST(str_name, type)
{
    {
        using t = aa::bb::Hello1<bool, int>;
        ASSERT_EQ(str_name_sv<t>, "aa::bb::Hello1<bool, int>");
        ASSERT_EQ(str_short_name_sv<t>, "Hello1<bool, int>");
        ASSERT_EQ(str_scope_name_sv<t>, "aa::bb");
    }
    {
        using t = aa::bb::Hello2<bool, int>::World2<int>;
        ASSERT_EQ(str_name_sv<t>, "aa::bb::Hello2<bool, int>::World2<int>");
        ASSERT_EQ(str_short_name_sv<t>, "World2<int>");
        ASSERT_EQ(str_scope_name_sv<t>, "aa::bb::Hello2<bool, int>");
    }
}

TEST(str_name, value)
{
    {
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest1::A1>>, "aa::bb::ETest1::A1");
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest1::B1>>, "aa::bb::ETest1::B1");
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest1::C1>>, "aa::bb::ETest1::C1");
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest1(4)>>, "(aa::bb::ETest1)4");

        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest2::A2>>, "aa::bb::A2");
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest2::B2>>, "aa::bb::B2");
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest2::C2>>, "aa::bb::C2");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

        // NOLINTNEXTLINE(-Wenum-constexpr-conversion)
        ASSERT_EQ(str_name_sv<typify<aa::bb::ETest2(4)>>, "(aa::bb::ETest2)4");

#pragma GCC diagnostic pop
    }

    {
        ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest1::A1>>, "A1");
        ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest1::B1>>, "B1");
        ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest1::C1>>, "C1");
        // has assertion failure
        // ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest1(4)>>, "(aa::bb::ETest1)4");

        ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest2::A2>>, "A2");
        ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest2::B2>>, "B2");
        ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest2::C2>>, "C2");
        // has assertion failure
        // ASSERT_EQ(str_short_name_sv<typify<aa::bb::ETest2(4)>>, "(aa::bb::ETest2)4");
    }

    {
        ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest1::A1>>, "aa::bb::ETest1");
        ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest1::B1>>, "aa::bb::ETest1");
        ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest1::C1>>, "aa::bb::ETest1");
        // has assertion failure
        // ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest1(4)>>, "(aa::bb::ETest1)4");

        ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest2::A2>>, "aa::bb");
        ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest2::B2>>, "aa::bb");
        ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest2::C2>>, "aa::bb");
        // has assertion failure
        // ASSERT_EQ(str_scope_name_sv<typify<aa::bb::ETest1(4)>>, "(aa::bb::ETest1)4");
    }
}

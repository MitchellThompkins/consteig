#include <gtest/gtest.h>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(sgn_function, sgn_test)
{
    static constexpr double a{sgn(100.0)};
    static constexpr double aAnswer{1.0};
    static_assert(equalWithin(a, aAnswer, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(a, aAnswer, CONSTEIG_TEST_TOLERANCE));

    static constexpr double b{sgn(-100.0)};
    static constexpr double bAnswer{-1.0};
    static_assert(equalWithin(b, bAnswer, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(b, bAnswer, CONSTEIG_TEST_TOLERANCE));

    static constexpr double c{sgn(0.0)};
    static constexpr double cAnswer{0.0};
    static_assert(equalWithin(c, cAnswer, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(c, cAnswer, CONSTEIG_TEST_TOLERANCE));

    static constexpr int d{sgn(100)};
    static constexpr int dAnswer{1};
    static_assert(d == dAnswer, MSG);
    ASSERT_TRUE(d == dAnswer);

    static constexpr int e{sgn(-100)};
    static constexpr int eAnswer{-1};
    static_assert(e == eAnswer, MSG);
    ASSERT_TRUE(e == eAnswer);

    static constexpr int f{sgn(0)};
    static constexpr int fAnswer{0};
    static_assert(f == fAnswer, MSG);
    ASSERT_TRUE(f == fAnswer);
}

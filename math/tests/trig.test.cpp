#include <gtest/gtest.h>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

// ---- sin tests ----

TEST(sin_function, sin_zero)
{
    static constexpr double val = consteig::sin(0.0);
    static_assert(val == 0.0, MSG);
    ASSERT_EQ(val, 0.0);
}

TEST(sin_function, sin_pi_over_6)
{
    static constexpr double pi_6 = PI_CONST / 6.0;
    static constexpr double val = consteig::sin(pi_6);
    static_assert(equalWithin(val, 0.5, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 0.5, CONSTEIG_TEST_TOLERANCE));
}

TEST(sin_function, sin_pi_over_2)
{
    static constexpr double pi_2 = PI_CONST / 2.0;
    static constexpr double val = consteig::sin(pi_2);
    static_assert(equalWithin(val, 1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 1.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(sin_function, sin_pi)
{
    static constexpr double val = consteig::sin(PI_CONST);
    static_assert(equalWithin(val, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 0.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(sin_function, sin_negative_pi_over_2)
{
    static constexpr double val = consteig::sin(-PI_CONST / 2.0);
    static_assert(equalWithin(val, -1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, -1.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(sin_function, sin_two_pi)
{
    static constexpr double val = consteig::sin(2.0 * PI_CONST);
    static_assert(equalWithin(val, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 0.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(sin_function, sin_integer)
{
    static constexpr double val = consteig::sin(1);
    static_assert(equalWithin(val, 0.841470984808, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(equalWithin(val, 0.841470984808, CONSTEIG_TEST_TOLERANCE));
}

TEST(sin_function, sin_float)
{
    static constexpr float val = consteig::sin(static_cast<float>(PI_CONST) / 2.0f);
    static_assert(equalWithin(val, 1.0f, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 1.0f, CONSTEIG_FLOAT_TEST_TOLERANCE));
}

// ---- cos tests ----

TEST(cos_function, cos_zero)
{
    static constexpr double val = consteig::cos(0.0);
    static_assert(val == 1.0, MSG);
    ASSERT_EQ(val, 1.0);
}

TEST(cos_function, cos_pi_over_3)
{
    static constexpr double pi_3 = PI_CONST / 3.0;
    static constexpr double val = consteig::cos(pi_3);
    static_assert(equalWithin(val, 0.5, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 0.5, CONSTEIG_TEST_TOLERANCE));
}

TEST(cos_function, cos_pi_over_2)
{
    static constexpr double pi_2 = PI_CONST / 2.0;
    static constexpr double val = consteig::cos(pi_2);
    static_assert(equalWithin(val, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 0.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(cos_function, cos_pi)
{
    static constexpr double val = consteig::cos(PI_CONST);
    static_assert(equalWithin(val, -1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, -1.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(cos_function, cos_negative_pi)
{
    static constexpr double val = consteig::cos(-PI_CONST);
    static_assert(equalWithin(val, -1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, -1.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(cos_function, cos_two_pi)
{
    static constexpr double val = consteig::cos(2.0 * PI_CONST);
    static_assert(equalWithin(val, 1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, 1.0, CONSTEIG_TEST_TOLERANCE));
}

TEST(cos_function, cos_integer)
{
    static constexpr double val = consteig::cos(1);
    static_assert(equalWithin(val, 0.540302305868, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(equalWithin(val, 0.540302305868, CONSTEIG_TEST_TOLERANCE));
}

TEST(cos_function, cos_float)
{
    static constexpr float val = consteig::cos(static_cast<float>(PI_CONST));
    static_assert(equalWithin(val, -1.0f, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(equalWithin(val, -1.0f, CONSTEIG_FLOAT_TEST_TOLERANCE));
}

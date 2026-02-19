#include <gtest/gtest.h>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

static constexpr double kThresh{1e-9};

TEST(power_function, pow_basic) {
    static constexpr double a{consteig::pow(5.3, 6)};
    static constexpr double aAnswer{22164.361129};
    static_assert(compareFloats(a, aAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(a, aAnswer, kThresh));

    static constexpr double b{consteig::pow(2.0, 3)};
    static constexpr double bAnswer{8.0};
    static_assert(compareFloats(b, bAnswer, kThresh), MSG);
    ASSERT_EQ(b, bAnswer);

    static constexpr double c{consteig::pow(3.0, 2)};
    static constexpr double cAnswer{9.0};
    static_assert(compareFloats(c, cAnswer, kThresh), MSG);
    ASSERT_EQ(c, cAnswer);
}

TEST(power_function, pow_zero_base) {
    static constexpr double val1 = consteig::pow(0.0, 1);
    ASSERT_EQ(val1, 0.0);

    static constexpr double val2 = consteig::pow(0.0, 5);
    ASSERT_EQ(val2, 0.0);
}

TEST(power_function, pow_zero_exponent) {
    static constexpr double val1 = consteig::pow(5.5, 0);
    ASSERT_EQ(val1, 1.0);

    static constexpr double val2 = consteig::pow(-3.2, 0);
    ASSERT_EQ(val2, 1.0);

    static constexpr double val3 = consteig::pow(0.0, 0);  // usually 1
    ASSERT_EQ(val3, 1.0);
}

TEST(power_function, pow_negative_base) {
    // (-2)^2 = 4
    static constexpr double val1 = consteig::pow(-2.0, 2);
    ASSERT_EQ(val1, 4.0);

    // (-2)^3 = -8
    static constexpr double val2 = consteig::pow(-2.0, 3);
    ASSERT_EQ(val2, -8.0);
}

TEST(power_function, pow_one_base) {
    static constexpr double val = consteig::pow(1.0, 100);
    ASSERT_EQ(val, 1.0);
}

TEST(power_function, pow_large) {
    // 2^10 = 1024
    static constexpr int val = consteig::pow(2, 10);
    ASSERT_EQ(val, 1024);
}

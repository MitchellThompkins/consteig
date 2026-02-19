#include <gtest/gtest.h>

#include <limits>

#include "constmath.hpp"
#include "test_tools.hpp"

static constexpr double kThresh{0.00000001};

using namespace consteig;

TEST(absolute_value, abs_basic) {
    static_assert(consteig::compareFloats(consteig::abs(0.0), 0.0, kThresh), MSG);

    ASSERT_FLOAT_EQ(consteig::abs(0.0), 0.0);
    ASSERT_FLOAT_EQ(consteig::abs(-0.0), 0.0);  // -0.0 abs is 0.0

    ASSERT_FLOAT_EQ(consteig::abs(1.0), 1.0);
    ASSERT_FLOAT_EQ(consteig::abs(-1.0), 1.0);

    static_assert(consteig::abs(0U) == 0U, MSG);
    ASSERT_EQ(consteig::abs(0U), 0U);
    static_assert(consteig::abs(0) == 0, MSG);
    ASSERT_EQ(consteig::abs(0), 0);

    static_assert(consteig::abs(1U) == 1U, MSG);
    ASSERT_EQ(consteig::abs(1U), 1U);
    static_assert(consteig::abs(1) == 1, MSG);
    ASSERT_EQ(consteig::abs(1), 1);
    static_assert(consteig::abs(-1) == 1, MSG);
    ASSERT_EQ(consteig::abs(-1), 1);
}

TEST(absolute_value, abs_limits_double) {
    constexpr double max_d = std::numeric_limits<double>::max();
    constexpr double min_d = std::numeric_limits<double>::min();
    constexpr double low_d = std::numeric_limits<double>::lowest();

    static_assert(consteig::compareFloats(consteig::abs(max_d), max_d, kThresh), MSG);
    ASSERT_FLOAT_EQ(consteig::abs(max_d), max_d);
    ASSERT_FLOAT_EQ(consteig::abs(-max_d), max_d);

    static_assert(consteig::compareFloats(consteig::abs(min_d), min_d, kThresh), MSG);
    ASSERT_FLOAT_EQ(consteig::abs(min_d), min_d);
    ASSERT_FLOAT_EQ(consteig::abs(-min_d), min_d);

    static_assert(consteig::compareFloats(consteig::abs(low_d), max_d, kThresh), MSG);  // lowest is -max
    ASSERT_FLOAT_EQ(consteig::abs(low_d), max_d);
}

TEST(absolute_value, abs_limits_int) {
    constexpr int max_i = std::numeric_limits<int>::max();
    constexpr int min_i = std::numeric_limits<int>::min() + 1;  // abs(min) might overflow for 2's complement min

    static_assert(consteig::abs(max_i) == max_i, MSG);
    ASSERT_EQ(consteig::abs(max_i), max_i);
    static_assert(consteig::abs(-max_i) == max_i, MSG);
    ASSERT_EQ(consteig::abs(-max_i), max_i);
    static_assert(consteig::abs(min_i) == -min_i, MSG);
    ASSERT_EQ(consteig::abs(min_i), -min_i);
}

TEST(absolute_value, abs_long_double) {
    constexpr long double val = -123.456789L;
    constexpr long double expected = 123.456789L;

    // Using a slightly looser threshold for long double just in case, or same one if precision allows
    static_assert(consteig::abs(val) > 0, "abs should be positive");
    static_assert(consteig::compareFloats(consteig::abs(val), expected, 1e-9L), MSG);

    ASSERT_NEAR(static_cast<double>(consteig::abs(val)), static_cast<double>(expected), 1e-9);
}

#include <gtest/gtest.h>

#include "constmath.hpp"

static constexpr double kThresh {0.0001};

TEST(power_function, power_test)
{
    static constexpr double a {consteig::pow(5.3,6)};
    static constexpr double aAnswer {22164.361129};
    static_assert(compareFloats(a, aAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(a, aAnswer, kThresh));
}

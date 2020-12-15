#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "constmath.hpp"

using namespace consteig;

static constexpr double kThresh {0.0001};

TEST(power_function, power_test)
{
    static constexpr double a {consteig::pow(5.3,6)};
    static constexpr double aAnswer {22164.361129};
    static_assert(compareFloats(a, aAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(a, aAnswer, kThresh));
}

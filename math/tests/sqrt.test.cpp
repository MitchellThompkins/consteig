#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "constmath.hpp"

using namespace consteig;

static constexpr double kThresh {0.00000001};

TEST(sqrt_function, sqrt_test)
{
    static constexpr double a {sqrt(4.2375)};
    static constexpr double aAnswer {2.05851888};
    static_assert(compareFloats(a, aAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(a, aAnswer, kThresh));

    static constexpr double b {sqrt(58582.28485)};
    static constexpr double bAnswer {242.037775667};
    static_assert(compareFloats(b, bAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(b, bAnswer, kThresh));

    static constexpr int c {sqrt(16)};
    static constexpr int cAnswer {4};
    static_assert(c==cAnswer);
    ASSERT_TRUE(c==cAnswer);

    static constexpr int d {sqrt(81)};
    static constexpr int dAnswer {9};
    static_assert(d==dAnswer);
    ASSERT_TRUE(d==dAnswer);
}

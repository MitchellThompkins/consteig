#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "constmath.hpp"

using namespace consteig;

static constexpr double kThresh {std::numeric_limits<double>::epsilon()};

TEST(sgn_function, sgn_test)
{
    static constexpr double a {sgn(100.0)};
    static constexpr double aAnswer {1.0};
    static_assert(compareFloats(a, aAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(a, aAnswer, kThresh));

    static constexpr double b {sgn(-100.0)};
    static constexpr double bAnswer {-1.0};
    static_assert(compareFloats(b, bAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(b, bAnswer, kThresh));

    static constexpr double c {sgn(0.0)};
    static constexpr double cAnswer {0.0};
    static_assert(compareFloats(c, cAnswer, kThresh), MSG);
    ASSERT_TRUE(compareFloats(c, cAnswer, kThresh));

    static constexpr int d {sgn(100)};
    static constexpr int dAnswer {1};
    static_assert(d==dAnswer, MSG);
    ASSERT_TRUE(d==dAnswer);
    
    static constexpr int e {sgn(-100)};
    static constexpr int eAnswer {-1};
    static_assert(e==eAnswer, MSG);
    ASSERT_TRUE(e==eAnswer);

    static constexpr int f {sgn(0)};
    static constexpr int fAnswer {0};
    static_assert(f==fAnswer, MSG);
    ASSERT_TRUE(f==fAnswer);
}

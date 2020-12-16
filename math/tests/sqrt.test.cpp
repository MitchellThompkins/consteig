#include <gtest/gtest.h>
#include "math.h"
#include "test_tools.hpp"

#include "constmath.hpp"

using namespace consteig;

TEST(sqrt_function, sqrt_test)
{
    static constexpr double a {4.2375};
    static constexpr double aConsteig {consteig::sqrt(a)};
    static constexpr double aStd {std::sqrt(a)};
    static_assert(aConsteig==aStd, MSG);
    ASSERT_TRUE(aConsteig==aStd);

    static constexpr double b {58582.28485};
    static constexpr double bConsteig {consteig::sqrt(b)};
    static constexpr double bStd {std::sqrt(b)};
    static_assert(bConsteig==bStd, MSG);
    ASSERT_TRUE(bConsteig==bStd);

    static constexpr int c {16};
    static constexpr int cConsteig {consteig::sqrt(c)};
    static constexpr int cAnswer {4};
    static_assert(cConsteig==cAnswer, MSG);
    ASSERT_TRUE(cConsteig==cAnswer);

    static constexpr int d {34225};
    static constexpr int dConsteig {consteig::sqrt(d)};
    static constexpr int dAnswer {185};
    static_assert(dConsteig==dAnswer, MSG);
    ASSERT_TRUE(dConsteig==dAnswer);

    static constexpr double e {58582.28485};
    static constexpr double eConsteig {consteig::sqrt(e)};
    static constexpr double eStd {std::sqrt(e)};
    static_assert(eConsteig==eStd, MSG);
    ASSERT_TRUE(eConsteig==eStd);

    //static constexpr long double f {-1.0};
    //static constexpr long double fConsteig {0};
    //static constexpr long double fStd {};
    //static_assert(eConsteig==eStd, MSG);
    //ASSERT_TRUE(eConsteig==eStd);
}

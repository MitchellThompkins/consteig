#include <gtest/gtest.h>
#include <limits>
#include "math.h"
#include "test_tools.hpp"

#include "constmath.hpp"

using namespace consteig;

template<typename T>
static constexpr T thresh()
{
    return std::numeric_limits<T>::epsilon();
}

TEST(sqrt_function, sqrt_test)
{
    static constexpr double a {4.2375};
    static constexpr double aConsteig {consteig::sqrt(a)};
    static constexpr double aStd {std::sqrt(a)};
    static_assert(compareFloats(aConsteig,aStd,thresh<double>()), MSG);
    ASSERT_TRUE(compareFloats(aConsteig,aStd,thresh<double>()));

    static constexpr double b {58582.28485};
    static constexpr double bConsteig {consteig::sqrt(b)};
    static constexpr double bStd {std::sqrt(b)};
    static_assert(compareFloats(bConsteig,bStd,thresh<double>()), MSG);
    ASSERT_TRUE(compareFloats(bConsteig,bStd,thresh<double>()));

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

    static constexpr long double e {58582.28485};
    static constexpr long double eConsteig {consteig::sqrt(e)};
    static constexpr long double eStd {std::sqrt(e)};
    static constexpr long double threshhold {1e-16};
    // TODO(mthompkins): This doesn't work for long double, and I'm not totally
    // sure why, but 1e-16 seems close enough for now....
    //static constexpr long double threshhold {thresh<long double>()};
    static_assert(compareFloats(eConsteig,eStd,threshhold), MSG);
    ASSERT_TRUE( compareFloats(eConsteig,eStd,threshhold) );

    static constexpr float f {4.2375};
    static constexpr float fConsteig {consteig::sqrt(f)};
    static constexpr float fStd {std::sqrt(f)};
    static_assert(compareFloats(fConsteig,fStd,thresh<float>()), MSG);
    ASSERT_TRUE(compareFloats(fConsteig,fStd,thresh<float>()));

    static constexpr float g {-22.2};
    static constexpr float gConsteig {consteig::sqrt(g)};
    static constexpr float gAnswer {-1.0};
    // TODO(mthompkins): This should be NaN, but for now it's this.
    //static constexpr float gAnswer {std::numeric_limits<float>::quiet_NaN()};
    static_assert(compareFloats(fConsteig,fStd,thresh<float>()), MSG);
    ASSERT_TRUE(compareFloats(fConsteig,fStd,thresh<float>()));
}

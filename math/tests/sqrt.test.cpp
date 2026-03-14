#include <gtest/gtest.h>

#include <limits>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

template <typename T> static constexpr T thresh()
{
    return std::numeric_limits<T>::epsilon();
}

TEST(sqrt_function, sqrt_basic_double)
{
    static constexpr double a{4.2375};
    static constexpr double aConsteig{consteig::sqrt(a)};
    static constexpr double aStd{2.058518885}; // approx
    static_assert(
        consteig::compareFloats(aConsteig, aStd, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(aConsteig, aStd, CONSTEIG_TEST_TOLERANCE);

    static constexpr double b{58582.28485};
    static constexpr double bConsteig{consteig::sqrt(b)};
    static constexpr double bStd{242.0377756673646}; // highly precise
    static_assert(
        consteig::compareFloats(bConsteig, bStd, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(bConsteig, bStd, CONSTEIG_TEST_TOLERANCE);
}

TEST(sqrt_function, sqrt_int)
{
    static constexpr int c{16};
    static constexpr int cConsteig{consteig::sqrt(c)};
    static_assert(cConsteig == 4, MSG);
    ASSERT_EQ(cConsteig, 4);

    static constexpr int d{34225};
    static constexpr int dConsteig{consteig::sqrt(d)};
    static_assert(dConsteig == 185, MSG);
    ASSERT_EQ(dConsteig, 185);

    // Test non-perfect square floor behavior for int
    static constexpr int e{17};
    static constexpr int eConsteig{consteig::sqrt(e)}; // floor(sqrt(17)) = 4
    static_assert(eConsteig == 4, MSG);
    ASSERT_EQ(eConsteig, 4);

    static constexpr int f{0};
    static constexpr int fConsteig{consteig::sqrt(f)};
    static_assert(fConsteig == 0, MSG);
    ASSERT_EQ(fConsteig, 0);

    static constexpr int g{1};
    static constexpr int gConsteig{consteig::sqrt(g)};
    static_assert(gConsteig == 1, MSG);
    ASSERT_EQ(gConsteig, 1);
}

TEST(sqrt_function, sqrt_long_double)
{
    static constexpr long double e{58582.28485L};
    static constexpr long double eConsteig{consteig::sqrt(e)};
    // Using simple double precision comparison as long double precision varies
    static_assert(consteig::compareFloats(static_cast<double>(eConsteig),
                                          242.0377756673646,
                                          CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_NEAR(static_cast<double>(eConsteig), 242.0377756673646,
                CONSTEIG_TEST_TOLERANCE);
}

TEST(sqrt_function, sqrt_float)
{
    static constexpr float f{4.2375f};
    static constexpr float fConsteig{consteig::sqrt(f)};
    static_assert(consteig::compareFloats(fConsteig, 2.0585189f,
                                          CONSTEIG_FLOAT_TEST_TOLERANCE),
                  MSG);
    ASSERT_NEAR(fConsteig, 2.0585189f, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(sqrt_function, csqrt_positive_float)
{
    static constexpr float g{22.2f};
    static constexpr Complex<float> gConsteig{consteig::csqrt(g)};
    static_assert(consteig::compareFloats(gConsteig.real, 4.711687595f,
                                          CONSTEIG_FLOAT_TEST_TOLERANCE),
                  MSG);
    static_assert(gConsteig.imag == 0.0f, MSG);
    ASSERT_NEAR(gConsteig.real, 4.711687595f, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_EQ(gConsteig.imag, 0.0f);
}

TEST(sqrt_function, csqrt_negative_float)
{
    static constexpr float g{-22.2f};
    static constexpr Complex<float> gConsteig{consteig::csqrt(g)};
    static_assert(gConsteig.real == 0.0f, MSG);
    static_assert(consteig::compareFloats(gConsteig.imag, 4.711687595f,
                                          CONSTEIG_FLOAT_TEST_TOLERANCE),
                  MSG);
    ASSERT_EQ(gConsteig.real, 0.0f);
    ASSERT_NEAR(gConsteig.imag, 4.711687595f, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(sqrt_function, sqrt_negative_float)
{
    float g{-4.0f};
    float gConsteig{consteig::sqrt(g)};
    ASSERT_TRUE(consteig::is_poison_nan(gConsteig));
}

TEST(sqrt_function, csqrt_positive_double)
{
    static constexpr double g{22.2};
    static constexpr Complex<double> gConsteig{consteig::csqrt(g)};
    static_assert(consteig::compareFloats(gConsteig.real, 4.711687595674061,
                                          CONSTEIG_TEST_TOLERANCE),
                  MSG);
    static_assert(gConsteig.imag == 0.0, MSG);
    ASSERT_NEAR(gConsteig.real, 4.711687595674061, CONSTEIG_TEST_TOLERANCE);
    ASSERT_EQ(gConsteig.imag, 0.0);
}

TEST(sqrt_function, csqrt_negative_double)
{
    static constexpr double g{-22.2};
    static constexpr Complex<double> gConsteig{consteig::csqrt(g)};
    static_assert(gConsteig.real == 0.0, MSG);
    static_assert(consteig::compareFloats(gConsteig.imag, 4.711687595674061,
                                          CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_EQ(gConsteig.real, 0.0);
    ASSERT_NEAR(gConsteig.imag, 4.711687595674061, CONSTEIG_TEST_TOLERANCE);
}

TEST(sqrt_function, sqrt_negative_double)
{
    double g{-4.0};
    double gConsteig{consteig::sqrt(g)};
    ASSERT_TRUE(consteig::is_poison_nan(gConsteig));
}

TEST(sqrt_function, sqrt_negative_int)
{
    int h{-5};
    int hConsteig{consteig::sqrt(h)};
    ASSERT_TRUE(consteig::is_poison_nan(hConsteig));
}

TEST(sqrt_function, csqrt_negative_int)
{
    static constexpr int h{-4};
    static constexpr Complex<int> hConsteig{consteig::csqrt(h)};
    static_assert(hConsteig.real == 0, MSG);
    static_assert(hConsteig.imag == 2, MSG);
    ASSERT_EQ(hConsteig.real, 0);
    ASSERT_EQ(hConsteig.imag, 2);
}

TEST(sqrt_function, sqrt_zero_one)
{
    static constexpr double z{0.0};
    static constexpr double zRes{consteig::sqrt(z)};
    static_assert(zRes == 0.0, MSG);
    ASSERT_EQ(zRes, 0.0);

    static constexpr double o{1.0};
    static constexpr double oRes{consteig::sqrt(o)};
    static_assert(oRes == 1.0, MSG);
    ASSERT_EQ(oRes, 1.0);
}

TEST(sqrt_function, sqrt_large)
{
    // sqrt(1e20) = 1e10
    static constexpr double l{1e20};
    static constexpr double lRes{consteig::sqrt(l)};
    static_assert(consteig::compareFloats(lRes, 1e10, LARGE_VAL_TOL), MSG);
    ASSERT_NEAR(lRes, 1e10,
                LARGE_VAL_TOL); // Allow some error for large numbers
}

#include <gtest/gtest.h>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(exp_function, exp_zero)
{
    static constexpr double val = consteig::exp(0.0);
    static_assert(val == 1.0, MSG);
    ASSERT_EQ(val, 1.0);
}

TEST(exp_function, exp_one)
{
    static constexpr double val = consteig::exp(1.0);
    static_assert(compareFloats(val, 2.718281828459, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(compareFloats(val, 2.718281828459, CONSTEIG_TEST_TOLERANCE));
}

TEST(exp_function, exp_negative)
{
    static constexpr double val = consteig::exp(-1.0);
    static_assert(
        compareFloats(val, 1.0 / 2.718281828459, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(
        compareFloats(val, 1.0 / 2.718281828459, CONSTEIG_TEST_TOLERANCE));
}

TEST(exp_function, exp_large)
{
    static constexpr double val = consteig::exp(4.5);
    static_assert(compareFloats(val, 90.0171313005, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(compareFloats(val, 90.0171313005, CONSTEIG_TEST_TOLERANCE));
}

TEST(exp_function, exp_integer)
{
    static constexpr double val = consteig::exp(2);
    static_assert(compareFloats(val, 7.38905609893, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(compareFloats(val, 7.38905609893, CONSTEIG_TEST_TOLERANCE));
}

TEST(exp_function, exp_complex_pure_imaginary)
{
    // exp(i * pi/2) = i
    static constexpr double pi_2 = 3.14159265358979323846 / 2.0;
    static constexpr Complex<double> val =
        consteig::exp(Complex<double>(0, pi_2));

    static_assert(compareFloats(val.real, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(compareFloats(val.imag, 1.0, CONSTEIG_TEST_TOLERANCE), MSG);

    ASSERT_NEAR(val.real, 0.0, CONSTEIG_TEST_TOLERANCE);
    ASSERT_NEAR(val.imag, 1.0, CONSTEIG_TEST_TOLERANCE);
}

TEST(exp_function, exp_complex_general)
{
    // exp(1 + i) = e * (cos(1) + i * sin(1))
    // e * cos(1) approx 2.718 * 0.5403 = 1.46869
    // e * sin(1) approx 2.718 * 0.8414 = 2.28735
    static constexpr Complex<double> val =
        consteig::exp(Complex<double>(1.0, 1.0));

    static_assert(
        compareFloats(val.real, 1.468693939915, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(
        compareFloats(val.imag, 2.287355287178, CONSTEIG_TEST_TOLERANCE), MSG);

    ASSERT_NEAR(val.real, 1.468693939915, CONSTEIG_TEST_TOLERANCE);
    ASSERT_NEAR(val.imag, 2.287355287178, CONSTEIG_TEST_TOLERANCE);
}

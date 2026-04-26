// Tests for math_backend.hpp and the refactored constmath.hpp
//
// Covers:
//  - consteig::equalWithin (moved from utilities.hpp to math_backend.hpp)
//  - consteig::abs(Complex<T>) (moved from complex.hpp to constmath.hpp)
//  - consteig::exp(Complex<T>) (moved from exp.hpp to constmath.hpp)
//
// These tests run for the built-in backend only (math tests are excluded when
// CONSTEIG_USE_GCEM is defined — see tests/CMakeLists.txt).

#include <cmath>
#include <gtest/gtest.h>
#include <limits>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

// ---------------------------------------------------------------------------
// equalWithin — boundary and edge-case coverage
// ---------------------------------------------------------------------------

TEST(math_backend, equal_within_identical_values)
{
    // Identical values are always within any positive tolerance
    static constexpr double a{3.14159265358979};
    static constexpr bool result = equalWithin(a, a, CONSTEIG_TEST_TOLERANCE);
    static_assert(result, MSG);
    ASSERT_TRUE(result);
}

TEST(math_backend, equal_within_exact_boundary_is_not_within)
{
    // |a - b| < thresh uses strict less-than, so diff == thresh returns false
    static constexpr double a{1.0};
    static constexpr double b{1.0 + 1e-6};
    static constexpr bool within_loose = equalWithin(a, b, 1e-5); // true
    static constexpr bool on_boundary  = equalWithin(a, b, 1e-6); // false (< not <=)
    static constexpr bool within_tight = equalWithin(a, b, 1e-7); // false
    static_assert(within_loose == true, MSG);
    static_assert(on_boundary == false, MSG);
    static_assert(within_tight == false, MSG);
    ASSERT_TRUE(within_loose);
    ASSERT_FALSE(on_boundary);
    ASSERT_FALSE(within_tight);
}

TEST(math_backend, equal_within_negative_values)
{
    // The function uses abs internally so sign should not matter
    static constexpr double a{-5.0};
    static constexpr double b{-5.0 + 1e-10};
    static constexpr bool result = equalWithin(a, b, CONSTEIG_TEST_TOLERANCE);
    static_assert(result, MSG);
    ASSERT_TRUE(result);
}

TEST(math_backend, equal_within_opposite_signs)
{
    // -1.0 and +1.0 differ by 2.0, so a tight tolerance should fail
    static constexpr double a{-1.0};
    static constexpr double b{1.0};
    static constexpr bool loose = equalWithin(a, b, 3.0);   // true
    static constexpr bool tight = equalWithin(a, b, 1.0);   // false (|diff|=2.0 >= 1.0)
    static_assert(loose, MSG);
    static_assert(!tight, MSG);
    ASSERT_TRUE(loose);
    ASSERT_FALSE(tight);
}

TEST(math_backend, equal_within_zero_values)
{
    static constexpr double zero{0.0};
    static constexpr bool result = equalWithin(zero, zero, CONSTEIG_TEST_TOLERANCE);
    static_assert(result, MSG);
    ASSERT_TRUE(result);
}

TEST(math_backend, equal_within_float_type)
{
    static constexpr float a{1.0f};
    static constexpr float b{1.0f + 1e-7f};
    static constexpr bool close = equalWithin(a, b, CONSTEIG_FLOAT_TEST_TOLERANCE);
    static_assert(close, MSG);
    ASSERT_TRUE(close);
}

TEST(math_backend, equal_within_mixed_threshold_type)
{
    // T=double, U=int threshold — exercises implicit conversion path
    static constexpr double a{5.0};
    static constexpr double b{5.5};
    static constexpr bool result = equalWithin(a, b, 1); // thresh converted to double
    static_assert(result, MSG);
    ASSERT_TRUE(result);
}

TEST(math_backend, equal_within_very_small_values)
{
    // Near-zero values: absolute tolerance still applies
    static constexpr double a{1e-12};
    static constexpr double b{2e-12};
    static constexpr bool within = equalWithin(a, b, 1e-11);
    static constexpr bool outside = equalWithin(a, b, 1e-13);
    static_assert(within, MSG);
    static_assert(!outside, MSG);
    ASSERT_TRUE(within);
    ASSERT_FALSE(outside);
}

// ---------------------------------------------------------------------------
// abs(Complex<T>) — moved from complex.hpp to constmath.hpp
// ---------------------------------------------------------------------------

TEST(math_backend, complex_abs_float_345_triangle)
{
    // |3 + 4i| = 5 (using float to exercise the float path)
    static constexpr Complex<float> c{3.0f, 4.0f};
    static constexpr float m = abs(c);
    static_assert(equalWithin(m, 5.0f, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(m, 5.0f, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(math_backend, complex_abs_double_345_triangle)
{
    // |3 + 4i| = 5 (double precision, separate from complex.test.cpp which
    // tests the same values but focuses on operator coverage)
    static constexpr Complex<double> c{3.0, 4.0};
    static constexpr double m = abs(c);
    static_assert(m == 5.0, MSG);
    ASSERT_EQ(m, 5.0);
}

TEST(math_backend, complex_abs_zero)
{
    static constexpr Complex<double> c{0.0, 0.0};
    static constexpr double m = abs(c);
    static_assert(m == 0.0, MSG);
    ASSERT_EQ(m, 0.0);
}

TEST(math_backend, complex_abs_purely_real)
{
    // |5 + 0i| = 5
    static constexpr Complex<double> c{5.0, 0.0};
    static constexpr double m = abs(c);
    static_assert(m == 5.0, MSG);
    ASSERT_EQ(m, 5.0);
}

TEST(math_backend, complex_abs_purely_imaginary)
{
    // |0 + 7i| = 7
    static constexpr Complex<double> c{0.0, 7.0};
    static constexpr double m = abs(c);
    static_assert(m == 7.0, MSG);
    ASSERT_EQ(m, 7.0);
}

TEST(math_backend, complex_abs_negative_parts)
{
    // |-3 + (-4)i| = 5 — abs should be non-negative
    static constexpr Complex<double> c{-3.0, -4.0};
    static constexpr double m = abs(c);
    static_assert(m == 5.0, MSG);
    ASSERT_EQ(m, 5.0);
}

TEST(math_backend, complex_abs_general_double)
{
    // |1 + 1i| = sqrt(2)
    static constexpr Complex<double> c{1.0, 1.0};
    static constexpr double m = abs(c);
    static constexpr double expected{1.41421356237309504};
    static_assert(equalWithin(m, expected, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(m, expected, CONSTEIG_TEST_TOLERANCE);
}

// ---------------------------------------------------------------------------
// exp(Complex<T>) — moved from exp.hpp to constmath.hpp
// ---------------------------------------------------------------------------

TEST(math_backend, complex_exp_zero)
{
    // exp(0 + 0i) = 1 + 0i
    static constexpr Complex<double> z{0.0, 0.0};
    static constexpr Complex<double> result = exp(z);
    static_assert(equalWithin(result.real, 1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(equalWithin(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(result.real, 1.0, CONSTEIG_TEST_TOLERANCE);
    ASSERT_NEAR(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE);
}

TEST(math_backend, complex_exp_purely_real_positive)
{
    // exp(2 + 0i) = e^2 + 0i
    static constexpr Complex<double> z{2.0, 0.0};
    static constexpr Complex<double> result = exp(z);
    static constexpr double e2{7.38905609893065022723};
    static_assert(equalWithin(result.real, e2, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(equalWithin(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(result.real, e2, CONSTEIG_TEST_TOLERANCE);
    ASSERT_NEAR(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE);
}

TEST(math_backend, complex_exp_purely_real_negative)
{
    // exp(-1 + 0i) = 1/e + 0i
    static constexpr Complex<double> z{-1.0, 0.0};
    static constexpr Complex<double> result = exp(z);
    static constexpr double inv_e{0.36787944117144232};
    static_assert(equalWithin(result.real, inv_e, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(equalWithin(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(result.real, inv_e, CONSTEIG_TEST_TOLERANCE);
    ASSERT_NEAR(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE);
}

TEST(math_backend, complex_exp_purely_imaginary_pi)
{
    // exp(0 + pi*i) = -1 + 0i  (Euler's formula)
    static constexpr double pi{3.14159265358979323846};
    static constexpr Complex<double> z{0.0, pi};
    static constexpr Complex<double> result = exp(z);
    static_assert(equalWithin(result.real, -1.0, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(equalWithin(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(result.real, -1.0, CONSTEIG_TEST_TOLERANCE);
    ASSERT_NEAR(result.imag, 0.0, CONSTEIG_TEST_TOLERANCE);
}

TEST(math_backend, complex_exp_float_type)
{
    // exp(0 + pi/2 * i) = 0 + 1i  (using float)
    static constexpr float pi_2{1.5707963267948966f};
    static constexpr Complex<float> z{0.0f, pi_2};
    static constexpr Complex<float> result = exp(z);
    static_assert(equalWithin(result.real, 0.0f, CONSTEIG_FLOAT_TEST_TOLERANCE),
                  MSG);
    static_assert(equalWithin(result.imag, 1.0f, CONSTEIG_FLOAT_TEST_TOLERANCE),
                  MSG);
    ASSERT_NEAR(result.real, 0.0f, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_NEAR(result.imag, 1.0f, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(math_backend, complex_exp_general_negative_real)
{
    // exp(-0.5 + 0.5i): real part exp(-0.5)*cos(0.5), imag exp(-0.5)*sin(0.5)
    static constexpr double re{-0.5};
    static constexpr double im{0.5};
    static constexpr Complex<double> z{re, im};
    static constexpr Complex<double> result = exp(z);
    // exp(-0.5) = 0.60653065971..., cos(0.5) = 0.87758256189..., sin(0.5) = 0.47942553860...
    static constexpr double expected_real{0.52779141450};
    static constexpr double expected_imag{0.28977610073};
    static_assert(equalWithin(result.real, expected_real, 1e-8), MSG);
    static_assert(equalWithin(result.imag, expected_imag, 1e-8), MSG);
    ASSERT_NEAR(result.real, expected_real, 1e-8);
    ASSERT_NEAR(result.imag, expected_imag, 1e-8);
}

// ---------------------------------------------------------------------------
// csqrt — verifies unqualified abs/sqrt dispatch still works correctly
// ---------------------------------------------------------------------------

TEST(math_backend, csqrt_zero)
{
    // csqrt(0) = 0 + 0i
    static constexpr Complex<double> result = consteig::csqrt(0.0);
    static_assert(result.real == 0.0, MSG);
    static_assert(result.imag == 0.0, MSG);
    ASSERT_EQ(result.real, 0.0);
    ASSERT_EQ(result.imag, 0.0);
}

TEST(math_backend, csqrt_positive_double_large)
{
    // csqrt(100.0) = 10.0 + 0i
    static constexpr Complex<double> result = consteig::csqrt(100.0);
    static_assert(equalWithin(result.real, 10.0, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(result.imag == 0.0, MSG);
    ASSERT_NEAR(result.real, 10.0, CONSTEIG_TEST_TOLERANCE);
    ASSERT_EQ(result.imag, 0.0);
}

TEST(math_backend, csqrt_negative_double_large)
{
    // csqrt(-100.0) = 0 + 10i
    static constexpr Complex<double> result = consteig::csqrt(-100.0);
    static_assert(result.real == 0.0, MSG);
    static_assert(equalWithin(result.imag, 10.0, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_EQ(result.real, 0.0);
    ASSERT_NEAR(result.imag, 10.0, CONSTEIG_TEST_TOLERANCE);
}

TEST(math_backend, csqrt_positive_int_perfect_square)
{
    // csqrt(9) = 3 + 0i
    static constexpr Complex<int> result = consteig::csqrt(9);
    static_assert(result.real == 3, MSG);
    static_assert(result.imag == 0, MSG);
    ASSERT_EQ(result.real, 3);
    ASSERT_EQ(result.imag, 0);
}

TEST(math_backend, csqrt_negative_int_large)
{
    // csqrt(-9) = 0 + 3i
    static constexpr Complex<int> result = consteig::csqrt(-9);
    static_assert(result.real == 0, MSG);
    static_assert(result.imag == 3, MSG);
    ASSERT_EQ(result.real, 0);
    ASSERT_EQ(result.imag, 3);
}
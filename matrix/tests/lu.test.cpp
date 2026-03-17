#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"
#include "test_tools.hpp"

using namespace consteig;

template <typename T, consteig::Size R, consteig::Size C>
static constexpr bool compareComplexMat(consteig::Matrix<Complex<T>, R, C> a,
                                        consteig::Matrix<Complex<T>, R, C> b,
                                        const T thresh)
{
    for (consteig::Size i{0}; i < R; i++)
    {
        for (consteig::Size j{0}; j < C; j++)
        {
            T dr = a(i, j).real - b(i, j).real;
            T di = a(i, j).imag - b(i, j).imag;
            T ar = (dr < 0) ? -dr : dr;
            T ai = (di < 0) ? -di : di;
            if (ar >= thresh || ai >= thresh)
                return false;
        }
    }
    return true;
}

TEST(lu_decomp, simple_system)
{
    static constexpr Size s{3};
    static constexpr Matrix<double, s, s> mat = {
        {{{1, 2, 3}, {0, 1, 4}, {5, 6, 0}}}};
    static constexpr Matrix<double, s, 1> b = {{{{1}, {2}, {3}}}};

    static constexpr LUMatrix<double, s> luRes = lu(mat);
    static constexpr Matrix<double, s, 1> x = lu_solve(luRes, b);

    static constexpr Matrix<double, s, 1> Ax = mat * x;
    static_assert(equalWithinMat(Ax, b, CONSTEIG_TEST_TOLERANCE), MSG);

    // Runtime check
    for (Size i = 0; i < s; ++i)
    {
        EXPECT_NEAR(Ax(i, 0), b(i, 0), CONSTEIG_TEST_TOLERANCE);
    }
}

TEST(lu_decomp, pivot_test)
{
    static constexpr Size s{2};
    // Need to swap rows because (0,0) is 0
    static constexpr Matrix<double, s, s> mat = {{{{0, 1}, {1, 1}}}};
    static constexpr Matrix<double, s, 1> b = {{{{2}, {3}}}};

    static constexpr LUMatrix<double, s> luRes = lu(mat);
    static constexpr Matrix<double, s, 1> x = lu_solve(luRes, b);

    static constexpr Matrix<double, s, 1> Ax = mat * x;
    static_assert(equalWithinMat(Ax, b, CONSTEIG_TEST_TOLERANCE), MSG);

    static_assert(luRes._p[0] == 1, "Should have swapped rows");
}

TEST(lu_decomp, complex_system)
{
    static constexpr Size s{2};
    // (1+i)x + 2y = 1
    // 0x + (1-i)y = i
    static constexpr Matrix<Complex<double>, s, s> mat = []() {
        Matrix<Complex<double>, s, s> m{};
        m(0, 0) = {1.0, 1.0};
        m(0, 1) = {2.0, 0.0};
        m(1, 0) = {0.0, 0.0};
        m(1, 1) = {1.0, -1.0};
        return m;
    }();
    static constexpr Matrix<Complex<double>, s, 1> b = []() {
        Matrix<Complex<double>, s, 1> m{};
        m(0, 0) = {1.0, 0.0};
        m(1, 0) = {0.0, 1.0};
        return m;
    }();

    static constexpr LUMatrix<Complex<double>, s> luRes = lu(mat);
    static constexpr Matrix<Complex<double>, s, 1> x = lu_solve(luRes, b);

    static constexpr Matrix<Complex<double>, s, 1> Ax = mat * x;

    static_assert(compareComplexMat(Ax, b, CONSTEIG_TEST_TOLERANCE), MSG);

    // Manual check at runtime
    for (Size i = 0; i < s; ++i)
    {
        EXPECT_NEAR(Ax(i, 0).real, b(i, 0).real, CONSTEIG_TEST_TOLERANCE);
        EXPECT_NEAR(Ax(i, 0).imag, b(i, 0).imag, CONSTEIG_TEST_TOLERANCE);
    }
}

TEST(lu_decomp, complex_identity)
{
    static constexpr Size s{2};
    static constexpr Matrix<Complex<double>, s, s> mat = []() {
        Matrix<Complex<double>, s, s> m{};
        m(0, 0) = {1.0, 0.0};
        m(0, 1) = {0.0, 0.0};
        m(1, 0) = {0.0, 0.0};
        m(1, 1) = {1.0, 0.0};
        return m;
    }();
    static constexpr Matrix<Complex<double>, s, 1> b = []() {
        Matrix<Complex<double>, s, 1> m{};
        m(0, 0) = {1.0, 2.0};
        m(1, 0) = {3.0, 4.0};
        return m;
    }();

    static constexpr LUMatrix<Complex<double>, s> luRes = lu(mat);
    static constexpr Matrix<Complex<double>, s, 1> x = lu_solve(luRes, b);

    static_assert(compareComplexMat(x, b, CONSTEIG_TEST_TOLERANCE), MSG);
}

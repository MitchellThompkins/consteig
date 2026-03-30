#include <gtest/gtest.h>

#include <cmath>

#include "test_tools.hpp"
#include <consteig/consteig.hpp>

using namespace consteig;

// Helper for complex sum
template <typename T, Size S>
constexpr Complex<T> sum(const Matrix<Complex<T>, S, 1> &vec)
{
    Complex<T> s{};
    for (Size i = 0; i < S; ++i)
    {
        s = s + vec(i, 0);
    }
    return s;
}

// Helper for complex product
template <typename T, Size S>
constexpr Complex<T> prod(const Matrix<Complex<T>, S, 1> &vec)
{
    Complex<T> p{1, 0};
    for (Size i = 0; i < S; ++i)
    {
        p = p * vec(i, 0);
    }
    return p;
}

template <typename T, Size S>
constexpr bool verify_values(const Matrix<Complex<T>, S, 1> &computed,
                             const Matrix<Complex<T>, S, 1> &expected,
                             T tol = static_cast<T>(CONSTEIG_TEST_TOLERANCE))
{
    bool matched[S] = {};
    for (Size i = 0; i < S; ++i)
    {
        matched[i] = false;
    }

    for (Size i = 0; i < S; ++i)
    {
        bool found = false;
        for (Size j = 0; j < S; ++j)
        {
            if (!matched[j])
            {
                if (consteig::abs(computed(i, 0) - expected(j, 0)) < tol)
                {
                    matched[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            return false;
        }
    }
    return true;
}

TEST(consteig_eigen, constexpr_eigenValues)
{
    static constexpr Size s{4};

    static constexpr Matrix<double, s, s> mat{
        {{-4.4529e-01, 4.9063e+00, -8.7871e-01, 6.3036e+00},
         {-6.3941e+00, 1.3354e+01, 1.6668e+00, 1.1945e+01},
         {3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
         {3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}}};

    static constexpr auto eigenValueTest{eigenvalues(mat)};

    // Compile-time verification: Sum of eigenvalues = Trace(A)
    static constexpr double tr = trace(mat);
    static constexpr auto sumEigs = sum(eigenValueTest);
    static_assert(consteig::abs(sumEigs.real - tr) <
                      static_cast<double>(CONSTEIG_TEST_TOLERANCE),
                  "Trace mismatch");
    static_assert(consteig::abs(sumEigs.imag) <
                      static_cast<double>(CONSTEIG_TEST_TOLERANCE),
                  "Trace imag mismatch");

    ASSERT_NEAR(sumEigs.real, tr, CONSTEIG_TEST_TOLERANCE);
}

TEST(consteig_eigen, symmetric_trace)
{
    static constexpr Size s{5};

    static constexpr consteig::Matrix<double, s, s> mat{{
        {-5, -4, 2, 1, 77.1},
        {-4, 5, 7, 8, 9.2},
        {2, 7, 0, -83, 2},
        {1, 8, -83, 3, 4},
        {77.1, 9.2, 2, 4, 2},
    }};

    static constexpr auto eigenValueTest{eigenvalues(mat)};

    // Compile-time verification: Sum of eigenvalues = Trace(A)
    static constexpr double tr = trace(mat);
    static constexpr auto sumEigs = sum(eigenValueTest);
    static_assert(consteig::abs(sumEigs.real - tr) <
                      static_cast<double>(CONSTEIG_TEST_TOLERANCE),
                  "Trace mismatch");

    ASSERT_NEAR(sumEigs.real, tr, CONSTEIG_TEST_TOLERANCE);
}

TEST(consteig_eigen, non_symmetric_complex_eigenvalues)
{
    static constexpr Size s{2};
    // Matrix with complex eigenvalues: [0, 1; -1, 0] -> +/- i
    static constexpr Matrix<double, s, s> mat{{{0.0, 1.0}, {-1.0, 0.0}}};

    static constexpr auto eigenValueTest{eigenvalues(mat)};

    // Compile-time check
    static constexpr auto sumEigs = sum(eigenValueTest);
    static constexpr double tr = trace(mat); // 0
    static_assert(consteig::abs(sumEigs.real - tr) < CONSTEIG_TEST_TOLERANCE,
                  "Trace mismatch");

    static constexpr auto prodEigs = prod(eigenValueTest); // i * -i = 1
    static constexpr double d = determinant(mat);          // 0 - (-1) = 1
    static_assert(consteig::abs(prodEigs.real - d) < CONSTEIG_TEST_TOLERANCE,
                  "Det mismatch");

    // Static verify
    static constexpr Matrix<Complex<double>, s, 1> expected{
        {{{0.0, 1.0}}, {{0.0, -1.0}}}};
    static_assert(verify_values(eigenValueTest, expected,
                                static_cast<double>(CONSTEIG_TEST_TOLERANCE)),
                  "Eigenvalue mismatch (constexpr)");

    // Runtime check
    bool found_i = false;
    bool found_neg_i = false;

    for (Size i = 0; i < s; ++i)
    {
        Complex<double> val = eigenValueTest(i, 0);
        EXPECT_NEAR(val.real, 0.0, CONSTEIG_TEST_TOLERANCE);
        if (std::abs(val.imag - 1.0) < CONSTEIG_TEST_TOLERANCE)
        {
            found_i = true;
        }
        if (std::abs(val.imag + 1.0) < CONSTEIG_TEST_TOLERANCE)
        {
            found_neg_i = true;
        }
    }
    EXPECT_TRUE(found_i);
    EXPECT_TRUE(found_neg_i);
}

TEST(consteig_eigen, non_symmetric_general)
{
    static constexpr Size s{3};
    static constexpr Matrix<double, s, s> mat{
        {{1.0, 2.0, 3.0}, {0.0, 1.0, 5.0}, {0.0, -2.0, 1.0}}};

    static constexpr auto eigenValueTest{eigenvalues(mat)};

    // Compile-time check
    static constexpr auto sumEigs =
        sum(eigenValueTest);                 // 1 + (1+3.16i) + (1-3.16i) = 3
    static constexpr double tr = trace(mat); // 1+1+1 = 3
    static_assert(consteig::abs(sumEigs.real - tr) < CONSTEIG_TEST_TOLERANCE,
                  "Trace mismatch");

    // Static verify
    // Eigenvalues are 1, 1 +/- sqrt(10)i  (approx 1 +/- 3.162277i)
    static constexpr Matrix<Complex<double>, s, 1> expected{
        {{{1.0, 0.0}}, {{1.0, 3.16227766}}, {{1.0, -3.16227766}}}};
    static_assert(verify_values(eigenValueTest, expected,
                                static_cast<double>(CONSTEIG_TEST_TOLERANCE)),
                  "Eigenvalue mismatch (constexpr)");

    bool found_1 = false;
    bool found_c1 = false;
    bool found_c2 = false;

    for (Size i = 0; i < s; ++i)
    {
        Complex<double> val = eigenValueTest(i, 0);
        if (std::abs(val.real - 1.0) < CONSTEIG_TEST_TOLERANCE)
        {
            if (std::abs(val.imag) < CONSTEIG_TEST_TOLERANCE)
            {
                found_1 = true;
            }
            else if (std::abs(val.imag - std::sqrt(10.0)) <
                     CONSTEIG_TEST_TOLERANCE)
            {
                found_c1 = true;
            }
            else if (std::abs(val.imag + std::sqrt(10.0)) <
                     CONSTEIG_TEST_TOLERANCE)
            {
                found_c2 = true;
            }
        }
    }

    EXPECT_TRUE(found_1);
    EXPECT_TRUE(found_c1);
    EXPECT_TRUE(found_c2);
}

template <typename T, Size S>
constexpr bool verify_eigenpairs_constexpr(
    const Matrix<T, S, S> &A, const Matrix<Complex<T>, S, 1> &evals,
    const Matrix<Complex<T>, S, S> &V, T tol)
{
    for (Size j = 0; j < S; ++j)
    {
        // Reject zero eigenvectors. The zero vector satisfies A*v == λ*v
        // trivially for any A and λ, so without this check a solver bug that
        // produces a zero column would pass the Av == λv test below undetected.
        T maxNorm{0};
        for (Size i = 0; i < S; ++i)
        {
            T r = consteig::abs(V(i, j).real);
            T im = consteig::abs(V(i, j).imag);
            maxNorm = (r > maxNorm) ? r : maxNorm;
            maxNorm = (im > maxNorm) ? im : maxNorm;
        }
        if (maxNorm <= tol)
        {
            return false;
        }
        Complex<T> lam = evals(j, 0);
        for (Size i = 0; i < S; ++i)
        {
            Complex<T> Av_i{0, 0};
            for (Size k = 0; k < S; ++k)
            {
                Av_i = Av_i + Complex<T>{A(i, k)} * V(k, j);
            }
            Complex<T> lv_i = lam * V(i, j);
            if (consteig::abs(Av_i.real - lv_i.real) > tol)
            {
                return false;
            }
            if (consteig::abs(Av_i.imag - lv_i.imag) > tol)
            {
                return false;
            }
        }
    }
    return true;
}

TEST(consteig_eigen, eigen_solver)
{
    static constexpr Size s{2};

    static constexpr Matrix<double, s, s> A{{{2.0, 1.0}, {1.0, 2.0}}};

    static constexpr EigenSolver<double, s> solver{A};

    static_assert(verify_eigenpairs_constexpr(A, solver.eigenvalues(),
                                              solver.eigenvectors(),
                                              CONSTEIG_TEST_TOLERANCE),
                  "Av = lambda*v relation failed at compile time");
}

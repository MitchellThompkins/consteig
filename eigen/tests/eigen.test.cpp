#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "../consteig.hpp"
#include "eigen_test_tools.hpp"

using namespace consteig;

// Helper for complex sum
template <typename T, Size S>
constexpr Complex<T> sum(const Matrix<Complex<T>, S, 1> &vec)
{
    Complex<T> s{};
    for (Size i = 0; i < S; ++i)
        s = s + vec(i, 0);
    return s;
}

// Helper for complex product
template <typename T, Size S>
constexpr Complex<T> prod(const Matrix<Complex<T>, S, 1> &vec)
{
    Complex<T> p{1, 0};
    for (Size i = 0; i < S; ++i)
        p = p * vec(i, 0);
    return p;
}

template <typename T, Size S>
constexpr bool verify_values(const Matrix<Complex<T>, S, 1> &computed,
                             const Matrix<Complex<T>, S, 1> &expected,
                             T tol = static_cast<T>(CONSTEIG_TEST_TOLERANCE))
{
    bool matched[S] = {};
    for (Size i = 0; i < S; ++i)
        matched[i] = false;

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
            return false;
    }
    return true;
}

TEST(consteig_eigen, constexpr_eigenValues)
{
    static constexpr Size s{4};

    static constexpr Matrix<double, s, s> mat{
        {{{-4.4529e-01, 4.9063e+00, -8.7871e-01, 6.3036e+00},
          {-6.3941e+00, 1.3354e+01, 1.6668e+00, 1.1945e+01},
          {3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
          {3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}}}};

    static constexpr auto eigenValueTest{eigvals(mat)};

    // Compile-time verification: Sum of eigenvalues = Trace(A)
    static constexpr double tr = trace(mat);
    static constexpr auto sumEigs = sum(eigenValueTest);
    static_assert(consteig::abs(sumEigs.real - tr) <
                      static_cast<double>(CONSTEIG_TEST_TOLERANCE),
                  "Trace mismatch");
    static_assert(consteig::abs(sumEigs.imag) <
                      static_cast<double>(CONSTEIG_TEST_TOLERANCE),
                  "Trace imag mismatch");

    // Runtime verification
    ASSERT_NEAR(sumEigs.real, tr, CONSTEIG_TEST_TOLERANCE);
}

TEST(consteig_eigen, symmetric_matrix)
{
    static constexpr Size s{5};

    static constexpr consteig::Matrix<double, s, s> mat{{{
        {-5, -4, 2, 1, 77.1},
        {-4, 5, 7, 8, 9.2},
        {2, 7, 0, -83, 2},
        {1, 8, -83, 3, 4},
        {77.1, 9.2, 2, 4, 2},
    }}};

    // Calculate using consteig (returns Complex<double>)
    static constexpr auto eigenValueTest{eigvals(mat)};

    // Compile-time verification: Sum of eigenvalues = Trace(A)
    static constexpr double tr = trace(mat);
    static constexpr auto sumEigs = sum(eigenValueTest);
    static_assert(consteig::abs(sumEigs.real - tr) <
                      static_cast<double>(CONSTEIG_TEST_TOLERANCE),
                  "Trace mismatch");

    // Calculate using Eigen (Reference)
    auto eigenMat = toEigen(mat);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double, s, s>> es(eigenMat);
    auto eigenValsRef = es.eigenvalues(); // Returns sorted vector

    // Copy consteig results to std::vector for sorting (extract real part since
    // symmetric)
    std::vector<double> myVals;
    for (Size i = 0; i < s; ++i)
    {
        EXPECT_NEAR(eigenValueTest(i, 0).imag, 0.0,
                    1e-9); // Symmetric -> real eigenvalues
        myVals.push_back(eigenValueTest(i, 0).real);
    }
    std::sort(myVals.begin(), myVals.end());

    // Compare
    for (Size i = 0; i < s; ++i)
    {
        EXPECT_NEAR(myVals[i], eigenValsRef[i], CONSTEIG_TEST_TOLERANCE)
            << "Mismatch at index " << i;
    }
}

TEST(consteig_eigen, non_symmetric_complex_eigenvalues)
{
    static constexpr Size s{2};
    // Matrix with complex eigenvalues: [0, 1; -1, 0] -> +/- i
    static constexpr Matrix<double, s, s> mat{{{{0.0, 1.0}, {-1.0, 0.0}}}};

    static constexpr auto eigenValueTest{eigvals(mat)};

    // Compile-time check
    static constexpr auto sumEigs = sum(eigenValueTest);
    static constexpr double tr = trace(mat); // 0
    static_assert(consteig::abs(sumEigs.real - tr) < 1e-9, "Trace mismatch");

    static constexpr auto prodEigs = prod(eigenValueTest); // i * -i = 1
    static constexpr double d = det(mat);                  // 0 - (-1) = 1
    static_assert(consteig::abs(prodEigs.real - d) < 1e-9, "Det mismatch");

    // Static verify
    static constexpr Matrix<Complex<double>, s, 1> expected{
        {{{{{0.0, 1.0}}}, {{{0.0, -1.0}}}}}};
    static_assert(verify_values(eigenValueTest, expected,
                                static_cast<double>(CONSTEIG_TEST_TOLERANCE)),
                  "Eigenvalue mismatch (constexpr)");

    // Check results
    bool found_i = false;
    bool found_neg_i = false;

    for (Size i = 0; i < s; ++i)
    {
        Complex<double> val = eigenValueTest(i, 0);
        EXPECT_NEAR(val.real, 0.0, CONSTEIG_TEST_TOLERANCE);
        if (std::abs(val.imag - 1.0) < CONSTEIG_TEST_TOLERANCE)
            found_i = true;
        if (std::abs(val.imag + 1.0) < CONSTEIG_TEST_TOLERANCE)
            found_neg_i = true;
    }
    EXPECT_TRUE(found_i);
    EXPECT_TRUE(found_neg_i);
}

TEST(consteig_eigen, non_symmetric_general)
{
    static constexpr Size s{3};
    static constexpr Matrix<double, s, s> mat{
        {{{1.0, 2.0, 3.0}, {0.0, 1.0, 5.0}, {0.0, -2.0, 1.0}}}};

    static constexpr auto eigenValueTest{eigvals(mat)};

    // Compile-time check
    static constexpr auto sumEigs =
        sum(eigenValueTest);                 // 1 + (1+3.16i) + (1-3.16i) = 3
    static constexpr double tr = trace(mat); // 1+1+1 = 3
    static_assert(consteig::abs(sumEigs.real - tr) < 1e-9, "Trace mismatch");

    // Static verify
    // Eigenvalues are 1, 1 +/- sqrt(10)i  (approx 1 +/- 3.162277i)
    static constexpr Matrix<Complex<double>, s, 1> expected{
        {{{{{1.0, 0.0}}}, {{{1.0, 3.16227766}}}, {{{1.0, -3.16227766}}}}}};
    static_assert(verify_values(eigenValueTest, expected,
                                static_cast<double>(CONSTEIG_TEST_TOLERANCE)),
                  "Eigenvalue mismatch (constexpr)");

    std::vector<Complex<double>> vals;
    for (Size i = 0; i < s; ++i)
        vals.push_back(eigenValueTest(i, 0));

    bool found_1 = false;
    bool found_c1 = false;
    bool found_c2 = false;

    for (const auto &v : vals)
    {
        if (std::abs(v.real - 1.0) < CONSTEIG_TEST_TOLERANCE)
        {
            if (std::abs(v.imag) < CONSTEIG_TEST_TOLERANCE)
                found_1 = true;
            else if (std::abs(v.imag - std::sqrt(10.0)) <
                     CONSTEIG_TEST_TOLERANCE)
                found_c1 = true;
            else if (std::abs(v.imag + std::sqrt(10.0)) <
                     CONSTEIG_TEST_TOLERANCE)
                found_c2 = true;
        }
    }

    EXPECT_TRUE(found_1);
    EXPECT_TRUE(found_c1);
    EXPECT_TRUE(found_c2);
}

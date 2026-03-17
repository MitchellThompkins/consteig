#include <gtest/gtest.h>

#include "../eigen.hpp"
#include "eigen_test_tools.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(eigenvectors, simple_symmetric)
{
    static constexpr Size s{2};
    // A = [2 1]
    //     [1 2]
    static constexpr Matrix<double, s, s> A = []() {
        Matrix<double, s, s> m{};
        m(0, 0) = 2.0;
        m(0, 1) = 1.0;
        m(1, 0) = 1.0;
        m(1, 1) = 2.0;
        return m;
    }();

    static constexpr Matrix<Complex<double>, s, 1> evals = eigvals(A);
    // This static constexpr evaluation proves the algorithm runs at
    // compile-time
    static constexpr Matrix<Complex<double>, s, s> V = eigvecs(A, evals);

    // Eigenvalues should be 3 and 1.
    bool found_3 = false;
    Size idx_3 = 0;
    bool found_1 = false;
    Size idx_1 = 0;

    for (Size i = 0; i < s; ++i)
    {
        if (consteig::abs(evals(i, 0).real - 3.0) < CONSTEIG_TEST_TOLERANCE)
        {
            found_3 = true;
            idx_3 = i;
        }
        if (consteig::abs(evals(i, 0).real - 1.0) < CONSTEIG_TEST_TOLERANCE)
        {
            found_1 = true;
            idx_1 = i;
        }
    }

    EXPECT_TRUE(found_3 && found_1) << "Expected eigenvalues 3 and 1";

    // Eigenvector for 3 should be roughly [1/sqrt(2), 1/sqrt(2)] -> [0.707,
    // 0.707] or [-0.707, -0.707]
    double v3_0 = V(0, idx_3).real;
    double v3_1 = V(1, idx_3).real;
    // They should be equal in magnitude and sign
    EXPECT_NEAR(consteig::abs(v3_0), consteig::abs(v3_1),
                CONSTEIG_TEST_TOLERANCE)
        << "V3 elements magnitude mismatch";
    EXPECT_TRUE(v3_0 * v3_1 > 0) << "V3 elements should have same sign";

    // Eigenvector for 1 should be roughly [1/sqrt(2), -1/sqrt(2)]
    double v1_0 = V(0, idx_1).real;
    double v1_1 = V(1, idx_1).real;
    // They should be equal in magnitude but opposite in sign
    EXPECT_NEAR(consteig::abs(v1_0), consteig::abs(v1_1),
                CONSTEIG_TEST_TOLERANCE)
        << "V1 elements magnitude mismatch";
    EXPECT_TRUE(v1_0 * v1_1 < 0) << "V1 elements should have opposite sign";

    // Let's explicitly check Av = lambda * v for the first eigenvalue
    Complex<double> lam = evals(0, 0);
    Complex<double> Av0 =
        Complex<double>{A(0, 0)} * V(0, 0) + Complex<double>{A(0, 1)} * V(1, 0);
    Complex<double> Av1 =
        Complex<double>{A(1, 0)} * V(0, 0) + Complex<double>{A(1, 1)} * V(1, 0);

    Complex<double> lv0 = lam * V(0, 0);
    Complex<double> lv1 = lam * V(1, 0);

    EXPECT_NEAR(Av0.real, lv0.real, CONSTEIG_TEST_TOLERANCE)
        << "A*v = lambda*v mismatch (real)";
    EXPECT_NEAR(Av0.imag, lv0.imag, CONSTEIG_TEST_TOLERANCE)
        << "A*v = lambda*v mismatch (imag)";
    EXPECT_NEAR(Av1.real, lv1.real, CONSTEIG_TEST_TOLERANCE)
        << "A*v = lambda*v mismatch (real)";
    EXPECT_NEAR(Av1.imag, lv1.imag, CONSTEIG_TEST_TOLERANCE)
        << "A*v = lambda*v mismatch (imag)";
}

TEST(eigenvectors, complex_rotation)
{
    static constexpr Size s{2};
    // Rotation matrix: [ 0 -1 ]
    //                 [ 1  0 ]
    // Eigenvalues: i, -i
    static constexpr Matrix<double, s, s> A = []() {
        Matrix<double, s, s> m{};
        m(0, 1) = -1.0;
        m(1, 0) = 1.0;
        return m;
    }();

    static constexpr Matrix<Complex<double>, s, 1> evals = eigvals(A);
    static constexpr Matrix<Complex<double>, s, s> V = eigvecs(A, evals);

    // Verify Av = lambda * v for both eigenvalues
    for (Size j = 0; j < s; ++j)
    {
        Complex<double> lam = evals(j, 0);

        // Av
        Complex<double> Av0 = Complex<double>{A(0, 0)} * V(0, j) +
                              Complex<double>{A(0, 1)} * V(1, j);
        Complex<double> Av1 = Complex<double>{A(1, 0)} * V(0, j) +
                              Complex<double>{A(1, 1)} * V(1, j);

        // lambda * v
        Complex<double> lv0 = lam * V(0, j);
        Complex<double> lv1 = lam * V(1, j);

        EXPECT_NEAR(Av0.real, lv0.real, CONSTEIG_TEST_TOLERANCE)
            << "Av = lv mismatch for eval " << j << " (row 0, real)";
        EXPECT_NEAR(Av0.imag, lv0.imag, CONSTEIG_TEST_TOLERANCE)
            << "Av = lv mismatch for eval " << j << " (row 0, imag)";
        EXPECT_NEAR(Av1.real, lv1.real, CONSTEIG_TEST_TOLERANCE)
            << "Av = lv mismatch for eval " << j << " (row 1, real)";
        EXPECT_NEAR(Av1.imag, lv1.imag, CONSTEIG_TEST_TOLERANCE)
            << "Av = lv mismatch for eval " << j << " (row 1, imag)";
    }
}

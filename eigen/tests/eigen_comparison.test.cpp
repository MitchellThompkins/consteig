#include <gtest/gtest.h>

#include <random>

#include "../consteig.hpp"
#include "eigen_test_tools.hpp"

using namespace consteig;

// Runtime verification of the algorithm against Eigen on random matrices
// This ensures algorithmic correctness beyond the static constexpr cases.

template <Size S> void verify_symmetric_random(const int seed)
{
    // Generate random symmetric matrix
    std::mt19937 gen(seed); // Fixed seed
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    Matrix<double, S, S> mat;
    for (Size i = 0; i < S; ++i)
    {
        for (Size j = i; j < S; ++j)
        {
            double val = dist(gen);
            mat(i, j) = val;
            mat(j, i) = val;
        }
    }

    // Consteig
    auto res = eigvals(mat); // Runtime call to constexpr function
    auto vecs = eigvecs(mat, res);

    // Verify Eigenvector Invariant at runtime
    for (Size j = 0; j < S; ++j)
    {
        Complex<double> lam = res(j, 0);
        for (Size i = 0; i < S; ++i)
        {
            Complex<double> Av_i{0, 0};
            for (Size k = 0; k < S; ++k)
            {
                Av_i = Av_i + Complex<double>{mat(i, k)} * vecs(k, j);
            }
            Complex<double> lv_i = lam * vecs(i, j);
            EXPECT_NEAR(Av_i.real, lv_i.real, CONSTEIG_TEST_TOLERANCE)
                << "Symmetric eigenvector invariant (real) mismatch";
            EXPECT_NEAR(Av_i.imag, lv_i.imag, CONSTEIG_TEST_TOLERANCE)
                << "Symmetric eigenvector invariant (imag) mismatch";
        }
    }

    // Eigen
    Eigen::MatrixXd eigMat = toEigen(mat);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(eigMat);
    Eigen::VectorXd ref = es.eigenvalues();

    std::vector<double> calc;
    for (Size i = 0; i < S; ++i)
        calc.push_back(res(i, 0).real);
    std::sort(calc.begin(), calc.end());

    for (Size i = 0; i < S; ++i)
    {
        EXPECT_NEAR(calc[i], ref(i), CONSTEIG_TEST_TOLERANCE)
            << "Symmetric mismatch at index " << i;
    }
}

template <Size S> void verify_nonsymmetric_random(const int seed)
{
    // Generate random matrix
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-5.0, 5.0);

    Matrix<double, S, S> mat;
    for (Size i = 0; i < S; ++i)
    {
        for (Size j = i; j < S; ++j)
        {
            mat(i, j) = dist(gen);
        }
    }

    // Consteig
    auto res = eigvals(mat);
    auto vecs = eigvecs(mat, res);

    // Verify Eigenvector Invariant at runtime
    for (Size j = 0; j < S; ++j)
    {
        Complex<double> lam = res(j, 0);
        for (Size i = 0; i < S; ++i)
        {
            Complex<double> Av_i{0, 0};
            for (Size k = 0; k < S; ++k)
            {
                Av_i = Av_i + Complex<double>{mat(i, k)} * vecs(k, j);
            }
            Complex<double> lv_i = lam * vecs(i, j);
            EXPECT_NEAR(Av_i.real, lv_i.real, CONSTEIG_TEST_TOLERANCE)
                << "Nonsymmetric eigenvector invariant (real) mismatch";
            EXPECT_NEAR(Av_i.imag, lv_i.imag, CONSTEIG_TEST_TOLERANCE)
                << "Nonsymmetric eigenvector invariant (imag) mismatch";
        }
    }

    // Eigen
    Eigen::MatrixXd eigMat = toEigen(mat);
    Eigen::EigenSolver<Eigen::MatrixXd> es(eigMat);
    Eigen::VectorXcd ref = es.eigenvalues();

    // Comparison of complex eigenvalues is tricky (ordering).
    // We'll match each result to the closest reference.
    std::vector<bool> matched(S, false);

    for (Size i = 0; i < S; ++i)
    {
        double real = res(i, 0).real;
        double imag = res(i, 0).imag;

        bool found = false;
        double min_dist = MAX_SENTINEL_VAL;

        for (Size j = 0; j < S; ++j)
        {
            if (matched[j])
                continue;

            double d_real = real - ref(j).real();
            double d_imag = imag - ref(j).imag();
            double dist = std::sqrt(d_real * d_real + d_imag * d_imag);

            if (dist < CONSTEIG_TEST_TOLERANCE)
            { // Looser tolerance for iterative methods
                matched[j] = true;
                found = true;
                break;
            }
            if (dist < min_dist)
                min_dist = dist;
        }

        EXPECT_TRUE(found) << "Failed to match eigenvalue " << real << "+"
                           << imag << "i. Closest dist: " << min_dist;
    }
}

TEST(eigen_comparison, random_symmetric_3x3)
{
    for (int i{1}; i < 100; i++)
    {
        verify_symmetric_random<3>(i);
    }
}
TEST(eigen_comparison, random_symmetric_5x5)
{
    for (int i{1}; i < 100; i++)
    {
        verify_symmetric_random<5>(i);
    }
}
TEST(eigen_comparison, random_symmetric_8x8)
{
    for (int i{1}; i < 100; i++)
    {
        verify_symmetric_random<8>(i);
    }
}

TEST(eigen_comparison, random_nonsymmetric_3x3)
{
    for (int i{1}; i < 100; i++)
    {
        verify_nonsymmetric_random<3>(i);
    }
}
TEST(eigen_comparison, random_nonsymmetric_5x5)
{
    for (int i{1}; i < 100; i++)
    {
        verify_nonsymmetric_random<5>(i);
    }
}
TEST(eigen_comparison, random_nonsymmetric_8x8)
{
    for (int i{1}; i < 100; i++)
    {
        verify_nonsymmetric_random<8>(i);
    }
}

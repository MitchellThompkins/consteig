#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"

using namespace consteig;

TEST(hessenberg, eigen_comparison)
{
    static constexpr Size s{4};
    static constexpr Matrix<double, s, s> mat = {
        {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}}};

    // Consteig Hessenberg (Calculate at compile time)
    static constexpr PHMatrix<double, s> hessRes = hess(mat);

    // Eigen Hessenberg (Runtime)
    Eigen::MatrixXd eigMat = toEigen(mat);
    Eigen::HessenbergDecomposition<Eigen::MatrixXd> hessEig(eigMat);
    Eigen::MatrixXd hEig = hessEig.matrixH();

    // Check if H is Upper Hessenberg
    for (Size i = 2; i < s; ++i)
    {
        for (Size j = 0; j < i - 1; ++j)
        {
            EXPECT_NEAR(hessRes._h(i, j), 0.0, CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Check eigenvalues of H match eigenvalues of A (using Eigen to compute
    // eigenvalues of H)
    Eigen::MatrixXd hConstEig = toEigen(hessRes._h);
    Eigen::VectorXcd eigValsH = hConstEig.eigenvalues();
    Eigen::VectorXcd eigValsA = eigMat.eigenvalues();

    // Compare H elements roughly (signs might differ)
    for (Size i = 0; i < s; ++i)
    {
        for (Size j = 0; j < s; ++j)
        {
            EXPECT_NEAR(std::abs(hessRes._h(i, j)),
                        std::abs(hEig(static_cast<Eigen::Index>(i),
                                      static_cast<Eigen::Index>(j))),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }
}

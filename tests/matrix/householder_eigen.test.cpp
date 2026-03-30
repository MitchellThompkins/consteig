#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(householder, eigen_comparison)
{
    static constexpr Size s{4};
    static constexpr Matrix<double, s, s> mat = {
        {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}};

    // Consteig Householder (Calculate at compile time)
    static constexpr Matrix<double, s, s> houseMat = house(mat);

    // Check properties using Eigen (Runtime)
    Eigen::MatrixXd eigHouse = toEigen(houseMat);
    Eigen::MatrixXd eigMat = toEigen(mat);

    // 1. Is unitary?
    EXPECT_TRUE(eigHouse.isUnitary(CONSTEIG_TEST_TOLERANCE));

    // 2. Does it zero out elements below a(1,0)? (indices 2, 3...)
    // house() is designed for Hessenberg reduction, preserving row 0 and
    // creating zeros in col 0 below row 1.
    Eigen::MatrixXd result = eigHouse * eigMat;

    for (Size i = 2; i < s; ++i)
    {
        EXPECT_NEAR(result(static_cast<Eigen::Index>(i), 0), 0.0,
                    CONSTEIG_TEST_TOLERANCE);
    }

    // 3. Does it leave row 0 affected?
    // Actually P = diag(1, P') where P' acts on subvector.
    // So (P*A).row(0) == A.row(0) if P is block diag(1, ...).
    // Let's check P structure.
    EXPECT_NEAR(houseMat(0, 0), 1.0, CONSTEIG_TEST_TOLERANCE);
    for (Size i = 1; i < s; ++i)
    {
        EXPECT_NEAR(houseMat(0, i), 0.0, CONSTEIG_TEST_TOLERANCE);
        EXPECT_NEAR(houseMat(i, 0), 0.0, CONSTEIG_TEST_TOLERANCE);
    }
}

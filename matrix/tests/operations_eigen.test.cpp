#include <gtest/gtest.h>

#include "eigen_test_tools.hpp"
#include "operations.hpp"

using namespace consteig;

TEST(matrix, eigen_comparison)
{
    // Setup matrices
    Matrix<double, 3, 3> mat1 = {{{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}}};
    Matrix<double, 3, 3> mat2 = {{{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}}};

    // Eigen equivalents
    Eigen::Matrix3d eigMat1 = toEigen(mat1);
    Eigen::Matrix3d eigMat2 = toEigen(mat2);

    // Addition
    Matrix<double, 3, 3> sum = mat1 + mat2;
    Eigen::Matrix3d eigSum = eigMat1 + eigMat2;
    for (Size i = 0; i < 3; ++i)
    {
        for (Size j = 0; j < 3; ++j)
        {
            EXPECT_NEAR(sum(i, j),
                        eigSum(static_cast<Eigen::Index>(i),
                               static_cast<Eigen::Index>(j)),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Subtraction
    Matrix<double, 3, 3> diff = mat1 - mat2;
    Eigen::Matrix3d eigDiff = eigMat1 - eigMat2;
    for (Size i = 0; i < 3; ++i)
    {
        for (Size j = 0; j < 3; ++j)
        {
            EXPECT_NEAR(diff(i, j),
                        eigDiff(static_cast<Eigen::Index>(i),
                                static_cast<Eigen::Index>(j)),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Multiplication
    Matrix<double, 3, 3> prod = mat1 * mat2;
    Eigen::Matrix3d eigProd = eigMat1 * eigMat2;
    for (Size i = 0; i < 3; ++i)
    {
        for (Size j = 0; j < 3; ++j)
        {
            EXPECT_NEAR(prod(i, j),
                        eigProd(static_cast<Eigen::Index>(i),
                                static_cast<Eigen::Index>(j)),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Transpose
    Matrix<double, 3, 3> trans = transpose(mat1);
    Eigen::Matrix3d eigTrans = eigMat1.transpose();
    for (Size i = 0; i < 3; ++i)
    {
        for (Size j = 0; j < 3; ++j)
        {
            EXPECT_NEAR(trans(i, j),
                        eigTrans(static_cast<Eigen::Index>(i),
                                 static_cast<Eigen::Index>(j)),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Determinant
    double d = det(mat1); // 0 for this specific matrix (singular)
    double eigDet = eigMat1.determinant();
    EXPECT_NEAR(d, eigDet, CONSTEIG_TEST_TOLERANCE);

    Matrix<double, 2, 2> mat3 = {{{{1, 2}, {3, 4}}}};
    Eigen::Matrix2d eigMat3 = toEigen(mat3);
    EXPECT_NEAR(det(mat3), eigMat3.determinant(), CONSTEIG_TEST_TOLERANCE);
}

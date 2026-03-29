#ifndef EIGEN_TEST_TOOLS_HPP
#define EIGEN_TEST_TOOLS_HPP

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wundef"
#endif

#include <Eigen/Dense>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#include "../matrix/matrix.hpp"
#include "test_tools.hpp"

template <typename T, consteig::Size R, consteig::Size C>
Eigen::Matrix<T, R, C> toEigen(const consteig::Matrix<T, R, C> &mat)
{
    Eigen::Matrix<T, R, C> res;
    for (consteig::Size row = 0; row < R; ++row)
    {
        for (consteig::Size col = 0; col < C; ++col)
        {
            res(static_cast<Eigen::Index>(row),
                static_cast<Eigen::Index>(col)) = mat(row, col);
        }
    }
    return res;
}

template <typename T, consteig::Size R, consteig::Size C>
void expectNear(const consteig::Matrix<T, R, C> &a,
                const Eigen::Matrix<T, R, C> &b, double tol = 1e-4)
{
    for (consteig::Size row = 0; row < R; ++row)
    {
        for (consteig::Size col = 0; col < C; ++col)
        {
            EXPECT_NEAR(a(row, col),
                        b(static_cast<Eigen::Index>(row),
                          static_cast<Eigen::Index>(col)),
                        tol)
                << "Mismatch at (" << row << "," << col << ")";
        }
    }
}

#endif

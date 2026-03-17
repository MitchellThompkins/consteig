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
    for (consteig::Size i = 0; i < R; ++i)
    {
        for (consteig::Size j = 0; j < C; ++j)
        {
            res(static_cast<Eigen::Index>(i), static_cast<Eigen::Index>(j)) =
                mat(i, j);
        }
    }
    return res;
}

template <typename T, consteig::Size R, consteig::Size C>
void expectNear(const consteig::Matrix<T, R, C> &a,
                const Eigen::Matrix<T, R, C> &b, double tol = 1e-4)
{
    for (consteig::Size i = 0; i < R; ++i)
    {
        for (consteig::Size j = 0; j < C; ++j)
        {
            EXPECT_NEAR(
                a(i, j),
                b(static_cast<Eigen::Index>(i), static_cast<Eigen::Index>(j)),
                tol)
                << "Mismatch at (" << i << "," << j << ")";
        }
    }
}

#endif

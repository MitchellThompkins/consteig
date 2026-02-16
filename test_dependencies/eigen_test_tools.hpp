#ifndef EIGEN_TEST_TOOLS_HPP
#define EIGEN_TEST_TOOLS_HPP

#include <Eigen/Dense>
#include "test_tools.hpp"
#include "../matrix/matrix.hpp"

template<typename T, size_t R, size_t C>
Eigen::Matrix<T, R, C> toEigen(const consteig::Matrix<T, R, C>& mat) {
    Eigen::Matrix<T, R, C> res;
    for(size_t i = 0; i < R; ++i) {
        for(size_t j = 0; j < C; ++j) {
            res(i, j) = mat(i, j);
        }
    }
    return res;
}

template<typename T, size_t R, size_t C>
void expectNear(const consteig::Matrix<T, R, C>& a, const Eigen::Matrix<T, R, C>& b, double tol = 1e-4) {
    for(size_t i = 0; i < R; ++i) {
        for(size_t j = 0; j < C; ++j) {
            EXPECT_NEAR(a(i,j), b(i,j), tol) << "Mismatch at (" << i << "," << j << ")";
        }
    }
}

#endif

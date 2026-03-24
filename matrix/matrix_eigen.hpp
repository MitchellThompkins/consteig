#ifndef MATRIX_EIGEN_HPP
#define MATRIX_EIGEN_HPP

#include "../eigen/eigen.hpp"
#include "matrix.hpp"

namespace consteig
{

template <typename T, Size R, Size C>
constexpr Matrix<Complex<T>, R, 1> Matrix<T, R, C>::eigenvalues() const
{
    static_assert(R == C, "eigenvalues requires a square matrix");
    return consteig::eigenvalues(*this);
}

template <typename T, Size R, Size C>
constexpr Matrix<Complex<T>, R, R> Matrix<T, R, C>::eigenvectors(
    const Matrix<Complex<T>, R, 1> &evals) const
{
    static_assert(R == C, "eigenvectors requires a square matrix");
    return consteig::eigenvectors(*this, evals);
}

} // namespace consteig

#endif // MATRIX_EIGEN_HPP

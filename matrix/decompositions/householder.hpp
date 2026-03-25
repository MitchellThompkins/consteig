#ifndef HOUSEHOLDER_HPP
#define HOUSEHOLDER_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

///////////// FUNCTION DECLARATIONS /////////////
template <typename T, Size R, Size C>
constexpr Matrix<T, R, R> house(Matrix<T, R, C> a);

// Algorithm: Householder Reflection
// Computes a Householder reflector H = I - 2*v*v^T such that H*x = alpha*e1,
// where x is the first column of the input matrix (below the diagonal), e1 is
// the first standard basis vector, and alpha = -sign(x[1]) * ||x[1:]||_2.
//
// This zeros out all subdiagonal entries of the first column, which is used
// repeatedly by hessenberg reduction and QR decomposition.
//
// The reflector vector v is computed as:
//   alpha = -sign(x[1]) * ||x[1:]||
//   r     = sqrt(0.5 * (alpha^2 - x[1]*alpha))
//   v[1]  = (x[1] - alpha) / (2r)
//   v[i]  = x[i] / (2r)   for i > 1
//
// Reference: Golub & Van Loan, "Matrix Computations" (4th ed.), sec. 5
// See also:
// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf
///////////// IMPLEMENTATIONS /////////////
template <typename T, Size R, Size C>
constexpr Matrix<T, R, R> house(Matrix<T, R, C> a)
{
    static_assert(R == C, "Householder expects a square matrix");
    static_assert(is_float<T>(),
                  "Householder Reflection expects floating point");

    // Compute ||x[1:]||^2 (sum of squares of subdiagonal entries)
    T alphaSum{0};
    for (Size row{1}; row < R; row++)
    {
        alphaSum += (a(row, 0) * a(row, 0));
    }

    // If the subdiagonal is already zero, no reflection needed
    if (consteig::abs(alphaSum) < consteig::epsilon<T>())
    {
        return eye<T, R>();
    }

    // alpha = -sign(x[1]) * ||x[1:]||
    T sign =
        (a(1, 0) < static_cast<T>(0)) ? static_cast<T>(-1) : static_cast<T>(1);
    T alpha{static_cast<T>(-1) * sign * consteig::sqrt(alphaSum)};

    // r = sqrt(0.5 * (alpha^2 - x[1]*alpha)), the normalization factor for v
    T r_sq{static_cast<T>(0.5) * ((alpha * alpha) - (a(1, 0) * alpha))};

    if (consteig::abs(r_sq) < consteig::epsilon<T>())
    {
        return eye<T, R>();
    }

    T r = consteig::sqrt(r_sq);
    T oneOverTwoR{1 / (static_cast<T>(2) * r)};

    // Build the reflector vector v
    Matrix<T, R, 1> v{}; // Zero init
    for (Size row = 0; row < R; ++row)
    {
        v(row, 0) = 0;
    }

    v(1, 0) = (a(1, 0) - alpha) * oneOverTwoR;
    for (Size row{2}; row < R; row++)
    {
        v(row, 0) = a(row, 0) * oneOverTwoR;
    }

    // H = I - 2*v*v^T
    Matrix<T, R, R> p = eye<T, R>() - (static_cast<T>(2) * v * transpose(v));

    return p;
}

template <typename T> constexpr Matrix<T, 2, 2> house(Matrix<T, 2, 2> /*a*/)
{
    Matrix<T, 2, 2> ident = eye<T, 2>();
    ident(1, 1) = ident(1, 1) * static_cast<T>(-1);
    return ident;
}

} // namespace consteig
#endif

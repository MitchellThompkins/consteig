#ifndef LU_DECOMP_HPP
#define LU_DECOMP_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

/// @addtogroup decompositions
/// @{

/// @brief Result type for LU decomposition with partial pivoting.
///
/// Holds factors L, U, and permutation P such that PA = LU, where P is
/// represented as a pivot index array rather than a full matrix.
///
/// @tparam T  Scalar element type.
/// @tparam S  Matrix dimension.
///
/// @var LUMatrix::_l  Unit lower-triangular factor (S×S).
/// @var LUMatrix::_u  Upper-triangular factor (S×S).
/// @var LUMatrix::_p  Permutation vector: row `i` of the permuted matrix
///                    corresponds to row `_p[i]` of the original.
template <typename T, Size S> struct LUMatrix
{
    Matrix<T, S, S> _l;
    Matrix<T, S, S> _u;
    Size _p[S];
};

/// @brief LU decomposition with partial pivoting.
///
/// Factors a square matrix as PA = LU where P is a row permutation, L is
/// unit lower triangular, and U is upper triangular. Partial pivoting selects
/// the largest magnitude element as the pivot to control rounding error growth.
///
/// Used internally by @ref eigenvectors for inverse iteration. Can also be used
/// directly for solving linear systems via @ref lu_solve.
///
/// @tparam T  Scalar type (works with both real and @ref Complex types).
/// @tparam S  Matrix dimension.
/// @param  a  Square input matrix.
/// @return @ref LUMatrix containing `_l`, `_u`, and permutation `_p`.
// Algorithm: LU Decomposition with Partial Pivoting
// Factors a square matrix A such that PA = LU, where P is a permutation matrix,
// L is unit lower triangular, and U is upper triangular. Partial pivoting
// selects the largest magnitude entry in each column as the pivot, which
// controls the growth of rounding errors during elimination.
//
// Reference: Golub & Van Loan, "Matrix Computations" (4th ed.), sec. 3.4
template <typename T, Size S>
constexpr LUMatrix<T, S> lu(const Matrix<T, S, S> &a)
{
    LUMatrix<T, S> res{};
    res._u = a;
    res._l = eye<T, S>();
    for (Size row = 0; row < S; ++row)
    {
        res._p[row] = row;
    }

    for (Size diag = 0; diag < S; ++diag)
    {
        // Pivot
        Size max_row = diag;
        auto max_val = consteig::abs(res._u(diag, diag));
        for (Size search_row = diag + 1; search_row < S; ++search_row)
        {
            auto val = consteig::abs(res._u(search_row, diag));
            if (val > max_val)
            {
                max_val = val;
                max_row = search_row;
            }
        }

        if (max_row != diag)
        {
            // Swap rows in U
            for (Size col = 0; col < S; ++col)
            {
                T tmp = res._u(diag, col);
                res._u(diag, col) = res._u(max_row, col);
                res._u(max_row, col) = tmp;
            }
            // Swap rows in L (elements below diagonal)
            for (Size col = 0; col < diag; ++col)
            {
                T tmp = res._l(diag, col);
                res._l(diag, col) = res._l(max_row, col);
                res._l(max_row, col) = tmp;
            }
            // Swap pivots
            Size tmp_p = res._p[diag];
            res._p[diag] = res._p[max_row];
            res._p[max_row] = tmp_p;
        }

        for (Size row = diag + 1; row < S; ++row)
        {
            // Note: In inverse iteration, we might encounter nearly singular
            // matrices. We use a small epsilon to avoid exact division by zero.
            auto pivot_abs = consteig::abs(res._u(diag, diag));
            if (pivot_abs > 1e-30)
            {
                res._l(row, diag) = res._u(row, diag) / res._u(diag, diag);
                for (Size col = diag; col < S; ++col)
                {
                    res._u(row, col) = res._u(row, col) -
                                       res._l(row, diag) * res._u(diag, col);
                }
            }
        }
    }
    return res;
}

/// @brief Solve the linear system Ax = b given the LU factorization of A.
///
/// Uses the result of @ref lu to solve in two triangular passes:
/// 1. Forward substitution to solve Ly = Pb.
/// 2. Backward substitution to solve Ux = y.
///
/// Nearly singular systems (diagonal element of U below 1e-30) are handled
/// gracefully to support inverse iteration in @ref eigenvectors.
///
/// @tparam T  Scalar type.
/// @tparam S  System dimension.
/// @param  lu  LU factorization from @ref lu.
/// @param  b   Right-hand side vector (S×1).
/// @return Solution vector x (S×1) satisfying Ax = b.
// Solves Ax = b using the LU factorization PA = LU.
// 1. Solve Ly = Pb for y (Forward Substitution)
// 2. Solve Ux = y for x (Backward Substitution)
template <typename T, Size S>
constexpr Matrix<T, S, 1> lu_solve(const LUMatrix<T, S> &lu,
                                   const Matrix<T, S, 1> &b)
{
    // Solve Ly = Pb
    Matrix<T, S, 1> pb{};
    for (Size row = 0; row < S; ++row)
    {
        pb(row, 0) = b(lu._p[row], 0);
    }

    Matrix<T, S, 1> y{};
    for (Size row = 0; row < S; ++row)
    {
        T sum = static_cast<T>(0);
        for (Size col = 0; col < row; ++col)
        {
            sum = sum + lu._l(row, col) * y(col, 0);
        }
        y(row, 0) = pb(row, 0) - sum;
    }

    // Solve Ux = y
    Matrix<T, S, 1> x{};
    for (Size i = S; i > 0; --i)
    {
        Size row = i - 1;
        T sum = static_cast<T>(0);
        for (Size col = row + 1; col < S; ++col)
        {
            sum = sum + lu._u(row, col) * x(col, 0);
        }

        auto diag_abs = consteig::abs(lu._u(row, row));
        if (diag_abs > 1e-30)
        {
            x(row, 0) = (y(row, 0) - sum) / lu._u(row, row);
        }
        else
        {
            // If nearly singular, we use a very small value instead of zero to
            // encourage the "explosion" required by Inverse Iteration.
            x(row, 0) = (y(row, 0) - sum) / static_cast<T>(1e-30);
        }
    }
    return x;
}

/// @}  // addtogroup decompositions

} // namespace consteig

#endif

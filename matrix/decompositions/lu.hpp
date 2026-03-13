#ifndef LU_DECOMP_HPP
#define LU_DECOMP_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

template <typename T, Size S> struct LUMatrix
{
    Matrix<T, S, S> _l;
    Matrix<T, S, S> _u;
    Array<Size, S> _p;
};

// Algorithm: LU Decomposition with Partial Pivoting
// Factors a square matrix A such that PA = LU.
template <typename T, Size S>
constexpr LUMatrix<T, S> lu(const Matrix<T, S, S> &a)
{
    LUMatrix<T, S> res{};
    res._u = a;
    res._l = eye<T, S>();
    for (Size i = 0; i < S; ++i)
        res._p[i] = i;

    for (Size i = 0; i < S; ++i)
    {
        // Pivot
        Size max_row = i;
        auto max_val = consteig::abs(res._u(i, i));
        for (Size k = i + 1; k < S; ++k)
        {
            auto val = consteig::abs(res._u(k, i));
            if (val > max_val)
            {
                max_val = val;
                max_row = k;
            }
        }

        if (max_row != i)
        {
            // Swap rows in U
            for (Size j = 0; j < S; ++j)
            {
                T tmp = res._u(i, j);
                res._u(i, j) = res._u(max_row, j);
                res._u(max_row, j) = tmp;
            }
            // Swap rows in L (elements below diagonal)
            for (Size j = 0; j < i; ++j)
            {
                T tmp = res._l(i, j);
                res._l(i, j) = res._l(max_row, j);
                res._l(max_row, j) = tmp;
            }
            // Swap pivots
            Size tmp_p = res._p[i];
            res._p[i] = res._p[max_row];
            res._p[max_row] = tmp_p;
        }

        for (Size j = i + 1; j < S; ++j)
        {
            // Note: In inverse iteration, we might encounter nearly singular
            // matrices. We use a small epsilon to avoid exact division by zero.
            auto pivot_abs = consteig::abs(res._u(i, i));
            if (pivot_abs > 1e-30)
            {
                res._l(j, i) = res._u(j, i) / res._u(i, i);
                for (Size k = i; k < S; ++k)
                {
                    res._u(j, k) = res._u(j, k) - res._l(j, i) * res._u(i, k);
                }
            }
        }
    }
    return res;
}

// Solves Ax = b using the LU factorization PA = LU.
// 1. Solve Ly = Pb for y (Forward Substitution)
// 2. Solve Ux = y for x (Backward Substitution)
template <typename T, Size S>
constexpr Matrix<T, S, 1> lu_solve(const LUMatrix<T, S> &lu,
                                   const Matrix<T, S, 1> &b)
{
    // Solve Ly = Pb
    Matrix<T, S, 1> pb{};
    for (Size i = 0; i < S; ++i)
    {
        pb(i, 0) = b(lu._p[i], 0);
    }

    Matrix<T, S, 1> y{};
    for (Size i = 0; i < S; ++i)
    {
        T sum = static_cast<T>(0);
        for (Size j = 0; j < i; ++j)
        {
            sum = sum + lu._l(i, j) * y(j, 0);
        }
        y(i, 0) = pb(i, 0) - sum;
    }

    // Solve Ux = y
    Matrix<T, S, 1> x{};
    for (Size i = S; i > 0; --i)
    {
        Size idx = i - 1;
        T sum = static_cast<T>(0);
        for (Size j = idx + 1; j < S; ++j)
        {
            sum = sum + lu._u(idx, j) * x(j, 0);
        }

        auto diag_abs = consteig::abs(lu._u(idx, idx));
        if (diag_abs > 1e-30)
        {
            x(idx, 0) = (y(idx, 0) - sum) / lu._u(idx, idx);
        }
        else
        {
            // If nearly singular, we use a very small value instead of zero to
            // encourage the "explosion" required by Inverse Iteration.
            x(idx, 0) = (y(idx, 0) - sum) / static_cast<T>(1e-30);
        }
    }
    return x;
}

} // namespace consteig

#endif

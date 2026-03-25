#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "../math/constmath.hpp"
#include "matrix.hpp"

namespace consteig
{

/*
 * | Operation             | Operator    |
 * |-----------------------+-------------|
 * | Addition              | +           |
 * | Subtraction           | -           |
 * | Matrix Multiplication | *           |
 * | Scalar Multiplication | *           |
 * | Dot Product           | dot()       |
 * | Transpose             | transpose() |
 * | Create Diagonal       | diagonal()  |
 * | Create Identity       | eye()       |
 * | Euclidean Normal      | norm()     |
 * | Square Root           | sqrt()      |
 * | Determinant           | determinant() |
 */

// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf
///////////// IMPLEMENTATIONS /////////////

template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> operator+(const Matrix<T, R, C> &lhs,
                                    const Matrix<T, R, C> &rhs)
{
    Matrix<T, R, C> result{};

    for (Size row{0}; row < R; ++row)
    {
        for (Size col{0}; col < C; ++col)
        {
            result(row, col) = lhs(row, col) + rhs(row, col);
        }
    }

    return result;
}

template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> operator-(const Matrix<T, R, C> &lhs,
                                    const Matrix<T, R, C> &rhs)
{
    Matrix<T, R, C> result{};

    for (Size row{0}; row < R; ++row)
    {
        for (Size col{0}; col < C; ++col)
        {
            result(row, col) = lhs(row, col) - rhs(row, col);
        }
    }

    return result;
}

// Multiply two matrices
template <typename T, Size R1, Size C1, Size R2, Size C2>
constexpr Matrix<T, R1, C2> operator*(const Matrix<T, R1, C1> &lhs,
                                      const Matrix<T, R2, C2> &rhs)
{
    static_assert(C1 == R2, "Number of columns must equal number of rows");
    Matrix<T, R1, C2> result{};

    for (Size row{0}; row < R1; row++)
    {
        for (Size col{0}; col < C2; col++)
        {
            for (Size k{0}; k < C1; k++)
            {
                result(row, col) += lhs(row, k) * rhs(k, col);
            }
        }
    }

    return result;
}

// Multiply by a scalar
// todo(mthompkins): Figure out how to not make it possible to pass the scalar
// to either side
template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> operator*(const T &lhs, const Matrix<T, R, C> &rhs)
{
    Matrix<T, R, C> result{};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result(row, col) = lhs * rhs(row, col);
        }
    }

    return result;
}

// Multiply a 1XN by a Nx1 matrix
template <typename T, Size R, Size C>
constexpr T dot(const Matrix<T, R, C> &lhs, const Matrix<T, R, C> &rhs)
{
    static_assert(R == 1, "Dot Product expects two 1xN matrices");
    Matrix<T, 1, 1> product{lhs * transpose(rhs)};
    T result{product(0, 0)};

    return result;
}

template <typename T, Size R, Size C>
constexpr Matrix<T, C, R> transpose(const Matrix<T, R, C> &mat)
{
    Matrix<T, C, R> result{};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result(col, row) = mat(row, col);
        }
    }

    return result;
}

template <typename T, Size S> constexpr Matrix<T, S, S> diagonal(const T val)
{
    Matrix<T, S, S> result{};

    for (Size row{0}, col{0}; row < S; row++, col++)
    {
        result(row, col) = val;
    }

    return result;
}

template <typename T, Size S> constexpr Matrix<T, S, S> eye()
{
    return diagonal<T, S>(static_cast<T>(1));
}

// Euclidean normal of a matrix
template <typename T, Size R, Size C>
constexpr T norm(const Matrix<T, R, C> &mat)
{
    T result{};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result += (mat(row, col) * mat(row, col));
        }
    }

    return consteig::sqrt(result);
}

// 1-norm of a matrix (max column sum)
template <typename T, Size R, Size C>
constexpr T norm1(const Matrix<T, R, C> &mat)
{
    T max_sum{static_cast<T>(0)};
    for (Size col{0}; col < C; ++col)
    {
        T col_sum{static_cast<T>(0)};
        for (Size row{0}; row < R; ++row)
        {
            col_sum += consteig::abs(mat(row, col));
        }
        if (col_sum > max_sum)
        {
            max_sum = col_sum;
        }
    }
    return max_sum;
}

// Infinity-norm of a matrix (max row sum)
template <typename T, Size R, Size C>
constexpr T normInf(const Matrix<T, R, C> &mat)
{
    T max_sum{static_cast<T>(0)};
    for (Size row{0}; row < R; ++row)
    {
        T row_sum{static_cast<T>(0)};
        for (Size col{0}; col < C; ++col)
        {
            row_sum += consteig::abs(mat(row, col));
        }
        if (row_sum > max_sum)
        {
            max_sum = row_sum;
        }
    }
    return max_sum;
}

template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> sqrt(const Matrix<T, R, C> &mat)
{
    Matrix<T, R, C> result{};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result(row, col) = consteig::sqrt(mat(row, col));
        }
    }

    return result;
}

// Algorithm: Determinant (Laplace Expansion)
// Currently implemented using Laplace expansion (cofactor expansion).
// Note: This has factorial time complexity (O(n!)) and is only practical for
// very small matrices.
template <typename T, Size R, Size C>
constexpr T determinant(const Matrix<T, R, C> &mat)
{
    static_assert(R == C, "Can only find determinant of a square matrix");

    if constexpr (R == 1)
    {
        return mat(0, 0);
    }
    else if constexpr (R == 2)
    {
        return (mat(0, 0) * mat(1, 1)) - (mat(0, 1) * mat(1, 0));
    }
    else
    {
        T result{static_cast<T>(0)};
        for (Size col{0}; col < R; col++)
        {
            Matrix<T, R - 1, C - 1> submat{};
            for (Size row{1}; row < R; row++)
            {
                Size subj{0U};
                for (Size k{0}; k < R; k++)
                {
                    if (k == col)
                    {
                        continue;
                    }
                    submat(row - 1, subj) = mat(row, k);
                    subj++;
                }
            }
            T sign = (col % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
            result += (sign * mat(0, col) * determinant(submat));
        }
        return result;
    }
}

template <typename T, Size R, Size C>
constexpr T trace(const Matrix<T, R, C> &mat)
{
    static_assert(R == C, "Trace expects a square matrix");

    T result{static_cast<T>(0)};
    for (Size i{0}; i < R; ++i)
    {
        result += mat(i, i);
    }
    return result;
}

} // namespace consteig
#endif

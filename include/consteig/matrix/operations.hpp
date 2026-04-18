#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "../math/constmath.hpp"
#include "matrix.hpp"

namespace consteig
{

/// @addtogroup matrix
/// @{

// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf

/// @brief Element-wise matrix addition.
///
/// @tparam T  Scalar element type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  lhs  Left-hand operand.
/// @param  rhs  Right-hand operand.
/// @return New matrix where each element equals `lhs(i,j) + rhs(i,j)`.
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

/// @brief Element-wise matrix subtraction.
///
/// @tparam T  Scalar element type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  lhs  Left-hand operand.
/// @param  rhs  Right-hand operand.
/// @return New matrix where each element equals `lhs(i,j) - rhs(i,j)`.
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

/// @brief Matrix multiplication: (R1×C1) * (R2×C2) → (R1×C2).
///
/// @tparam T   Scalar element type.
/// @tparam R1  Rows of `lhs`.
/// @tparam C1  Columns of `lhs` (must equal `R2`).
/// @tparam R2  Rows of `rhs` (must equal `C1`).
/// @tparam C2  Columns of `rhs`.
/// @param  lhs  Left matrix.
/// @param  rhs  Right matrix.
/// @return Product matrix of dimension R1×C2.
/// @pre `C1 == R2` (enforced by `static_assert`).
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
            for (Size inner{0}; inner < C1; inner++)
            {
                result(row, col) += lhs(row, inner) * rhs(inner, col);
            }
        }
    }

    return result;
}

/// @brief Scalar-matrix multiplication.
///
/// @tparam T  Scalar and element type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  lhs  Scalar multiplier.
/// @param  rhs  Matrix to scale.
/// @return New matrix where each element equals `lhs * rhs(i,j)`.
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

/// @brief Dot product of two 1×N row vectors.
///
/// Computes `lhs * rhs^T` and returns the scalar result.
///
/// @tparam T  Scalar element type.
/// @tparam R  Must be 1 (enforced by `static_assert`).
/// @tparam C  Number of elements.
/// @param  lhs  First row vector (1×C).
/// @param  rhs  Second row vector (1×C).
/// @return Scalar dot product.
// Multiply a 1XN by a Nx1 matrix
template <typename T, Size R, Size C>
constexpr T dot(const Matrix<T, R, C> &lhs, const Matrix<T, R, C> &rhs)
{
    static_assert(R == 1, "Dot Product expects two 1xN matrices");
    Matrix<T, 1, 1> product{lhs * transpose(rhs)};
    T result{product(0, 0)};

    return result;
}

/// @brief Matrix transpose.
///
/// @tparam T  Scalar element type.
/// @tparam R  Number of rows in input (= columns in output).
/// @tparam C  Number of columns in input (= rows in output).
/// @param  mat  Input matrix.
/// @return New C×R matrix with rows and columns swapped.
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

/// @brief Create an S×S matrix with `val` on the main diagonal and zeros
/// elsewhere.
///
/// @tparam T  Scalar type.
/// @tparam S  Matrix dimension.
/// @param  val  Value to place on the diagonal.
/// @return S×S diagonal matrix.
template <typename T, Size S> constexpr Matrix<T, S, S> diagonal(const T val)
{
    Matrix<T, S, S> result{};

    for (Size row{0}, col{0}; row < S; row++, col++)
    {
        result(row, col) = val;
    }

    return result;
}

/// @brief Create an S×S identity matrix.
///
/// @tparam T  Scalar type.
/// @tparam S  Matrix dimension.
/// @return S×S identity matrix (1 on diagonal, 0 elsewhere).
///
/// @code
/// static constexpr auto I = consteig::eye<double, 3>();
/// @endcode
template <typename T, Size S> constexpr Matrix<T, S, S> eye()
{
    return diagonal<T, S>(static_cast<T>(1));
}

/// @brief Frobenius (Euclidean) norm: `sqrt(sum of squared elements)`.
///
/// @tparam T  Floating-point scalar type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  mat  Input matrix.
/// @return Non-negative Frobenius norm.
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

    return sqrt(result);
}

/// @brief 1-norm (maximum absolute column sum).
///
/// @tparam T  Scalar type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  mat  Input matrix.
/// @return Maximum sum of absolute values over all columns.
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
            col_sum += abs(mat(row, col));
        }
        if (col_sum > max_sum)
        {
            max_sum = col_sum;
        }
    }
    return max_sum;
}

/// @brief Infinity-norm (maximum absolute row sum).
///
/// @tparam T  Scalar type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  mat  Input matrix.
/// @return Maximum sum of absolute values over all rows.
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
            row_sum += abs(mat(row, col));
        }
        if (row_sum > max_sum)
        {
            max_sum = row_sum;
        }
    }
    return max_sum;
}

/// @brief Element-wise square root.
///
/// Applies @ref sqrt to every element. Each element must be
/// non-negative; a negative element triggers a compile-time error.
///
/// @tparam T  Floating-point scalar type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  mat  Input matrix.
/// @return New matrix with `result(i,j) = sqrt(mat(i,j))`.
template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> sqrt(const Matrix<T, R, C> &mat)
{
    Matrix<T, R, C> result{};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result(row, col) = sqrt(mat(row, col));
        }
    }

    return result;
}

/// @brief Determinant via Laplace (cofactor) expansion.
///
/// Computes the determinant recursively. Time complexity is O(n!), so this
/// is only practical for small matrices (n ≤ 4 or 5). Used internally by
/// @ref checkEigenValues only when `R <= 4`.
///
/// @tparam T  Scalar type.
/// @tparam R  Number of rows (must equal `C`).
/// @tparam C  Number of columns.
/// @param  mat  Square input matrix.
/// @return Determinant of `mat`.
/// @pre `R == C` (enforced by `static_assert`).
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
                for (Size src_col{0}; src_col < R; src_col++)
                {
                    if (src_col == col)
                    {
                        continue;
                    }
                    submat(row - 1, subj) = mat(row, src_col);
                    subj++;
                }
            }
            T sign = (col % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
            result += (sign * mat(0, col) * determinant(submat));
        }
        return result;
    }
}

/// @brief Trace: sum of diagonal elements.
///
/// @tparam T  Scalar type.
/// @tparam R  Number of rows (must equal `C`).
/// @tparam C  Number of columns.
/// @param  mat  Square input matrix.
/// @return Sum of `mat(i,i)` for all `i`.
/// @pre `R == C` (enforced by `static_assert`).
template <typename T, Size R, Size C>
constexpr T trace(const Matrix<T, R, C> &mat)
{
    static_assert(R == C, "Trace expects a square matrix");

    T result{static_cast<T>(0)};
    for (Size diag{0}; diag < R; ++diag)
    {
        result += mat(diag, diag);
    }
    return result;
}

/// @brief Compare two scalar values within an absolute tolerance.
///
/// Returns `true` if `|a - b| < thresh`. Does not use relative tolerance,
/// so be careful when comparing values with very different magnitudes.
///
/// @tparam T      Type of the values being compared.
/// @tparam U      Type of the threshold (converted to `T` internally).
/// @param  a      First value.
/// @param  b      Second value.
/// @param  thresh Absolute tolerance.
/// @return `true` if the values are within `thresh` of each other.
template <typename T, typename U>
constexpr bool equalWithin(T a, T b, U thresh)
{
    return abs(a - b) < static_cast<T>(thresh);
}

/// @brief Element-wise approximate equality within an absolute tolerance.
///
/// Returns `true` if every element satisfies
/// `|a(i,j) - b(i,j)| < thresh`. Prefer this over `operator==` for
/// floating-point matrices. Can also be called as a member function via
/// @ref Matrix::equalWithin.
///
/// @tparam T      Scalar element type.
/// @tparam R      Number of rows.
/// @tparam C      Number of columns.
/// @param  a      First matrix.
/// @param  b      Second matrix.
/// @param  thresh Absolute per-element tolerance.
/// @return `true` if all elements are within `thresh` of each other.
template <typename T, Size R, Size C>
constexpr bool equalWithinMat(const Matrix<T, R, C> &a,
                              const Matrix<T, R, C> &b, const T thresh)
{
    return a.equalWithin(b, thresh);
}

/// @}  // addtogroup matrix

} // namespace consteig
#endif

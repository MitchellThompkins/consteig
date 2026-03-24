#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../array/array.hpp"
#include "../math/functions/utilities.hpp"

namespace consteig
{

/// @defgroup matrix Matrix
/// @brief Fixed-size matrix type and arithmetic operations.
/// @{

/// @brief Fixed-size matrix with compile-time dimensions.
///
/// The primary container type for all consteig operations. All member
/// functions are `constexpr`, enabling full compile-time evaluation when
/// the matrix is declared `static constexpr`.
///
/// Storage is row-major: `_data[i][j]` holds row `i`, column `j`.
/// Element access uses `operator()(i, j)` (1-based indexing is **not** used;
/// indices run from `0` to `R-1` / `C-1`).
///
/// @tparam T  Scalar element type. Floating-point types (`float`, `double`,
///            `long double`) are required for eigensolver and decomposition
///            operations. Integer types are supported for pure arithmetic.
/// @tparam R  Number of rows (compile-time constant).
/// @tparam C  Number of columns (compile-time constant).
///
/// @code
/// // 2x2 matrix of doubles
/// static constexpr consteig::Matrix<double, 2, 2> A{{
///     {1.0, 2.0},
///     {3.0, 4.0}
/// }};
/// static constexpr double val = A(0, 1); // 2.0
/// @endcode
template <typename T, Size R, Size C> class Matrix
{
  public:
    /// @brief Access element at row `i`, column `j` (mutable).
    constexpr T &operator()(const Size i, const Size j)
    {
        return _data[i][j];
    }

    /// @brief Access element at row `i`, column `j` (read-only).
    constexpr const T &operator()(const Size i, const Size j) const
    {
        return _data[i][j];
    }

    // TODO(mthompkins): Need to handle the equal floats case
    /// @brief Exact element-wise equality. Prefer @ref equalWithinMat for floats.
    template <typename U>
    constexpr bool operator==(const Matrix<U, R, C> &rhs) const
    {
        for (Size i{0}; i < R; i++)
        {
            for (Size j{0}; j < C; j++)
            {
                if ((*this)(i, j) != rhs(i, j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    /// @brief Inequality (exact). Prefer negated @ref equalWithinMat for floats.
    template <typename U>
    constexpr bool operator!=(const Matrix<U, R, C> &rhs) const
    {
        return !(*this == rhs);
    }

    /// @brief Extract row `n` as a 1×C matrix.
    /// @param n  Zero-based row index. Must be `< R`.
    constexpr Matrix<T, 1, C> row(const Size n) const
    {
        Matrix<T, 1, C> result{};

        for (Size j{0}; j < C; j++)
        {
            result(0, j) = (*this)(n, j);
        }

        return result;
    }

    /// @brief Extract a contiguous subset of row `n`.
    ///
    /// @tparam startIndex  First column index (inclusive, compile-time).
    /// @tparam endIndex    Last column index (inclusive, compile-time).
    /// @param  n           Zero-based row index.
    /// @return 1×(endIndex-startIndex+1) matrix.
    // Get subset of row
    template <Size startIndex, Size endIndex>
    constexpr Matrix<T, 1, endIndex - startIndex + 1> row(const Size n) const
    {
        static_assert(C > startIndex, "startIndex cannot be larger than array");
        static_assert(C > endIndex, "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex,
                      "startIndex cannot be larger than endIndex");

        Matrix<T, 1, endIndex - startIndex + 1> result{};

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            result(0, i - startIndex) = (*this)(n, i);
        }

        return result;
    }

    /// @brief Extract column `n` as an R×1 matrix.
    /// @param n  Zero-based column index. Must be `< C`.
    constexpr Matrix<T, R, 1> col(const Size n) const
    {
        Matrix<T, R, 1> result{};

        for (Size i{0}; i < R; i++)
        {
            result(i, 0) = (*this)(i, n);
        }

        return result;
    }

    /// @brief Extract a contiguous subset of column `n`.
    ///
    /// @tparam startIndex  First row index (inclusive, compile-time).
    /// @tparam endIndex    Last row index (inclusive, compile-time).
    /// @param  n           Zero-based column index.
    /// @return (endIndex-startIndex+1)×1 matrix.
    // Get subset of column
    template <Size startIndex, Size endIndex>
    constexpr Matrix<T, endIndex - startIndex + 1, 1> col(const Size n) const
    {
        static_assert(R > startIndex, "startIndex cannot be larger than array");
        static_assert(R > endIndex, "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex,
                      "startIndex cannot be larger than endIndex");

        Matrix<T, endIndex - startIndex + 1, 1> result{};

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            result(i - startIndex, 0) = (*this)(i, n);
        }

        return result;
    }

    /// @brief Extract a rectangular submatrix.
    ///
    /// All indices are zero-based and inclusive. The returned matrix has
    /// dimensions `(x2-x1+1) × (y2-y1+1)`.
    ///
    /// @tparam x1  Top-left row index.
    /// @tparam y1  Top-left column index.
    /// @tparam x2  Bottom-right row index. Must be `>= x1`.
    /// @tparam y2  Bottom-right column index. Must be `>= y1`.
    // x1,y1,x2,y2 are indexes
    template <Size x1, Size y1, Size x2, Size y2>
    constexpr Matrix<T, x2 - x1 + 1, y2 - y1 + 1> sub() const
    {
        static_assert(x2 >= x1,
                      "Second x index must be bigger than the first.");
        static_assert(y2 >= y1,
                      "Second y index must be bigger than the first.");

        Matrix<T, x2 - x1 + 1, y2 - y1 + 1> result{};

        for (Size i{x1}; i <= x2; i++)
        {
            for (Size j{y1}; j <= y2; j++)
            {
                result(i - x1, j - y1) = (*this)(i, j);
            }
        }

        return result;
    }

    constexpr Matrix() = default;
    constexpr Matrix(const Matrix &) = default;
    constexpr Matrix(Matrix &&) = default;
    constexpr Matrix &operator=(const Matrix &) = default;
    constexpr Matrix &operator=(Matrix &&) = default;

    /// @brief Overwrite row `n` with the contents of `mat`.
    /// @param mat  1×C source matrix.
    /// @param n    Zero-based target row index.
    constexpr void setRow(const Matrix<T, 1, C> &mat, const Size n)
    {
        for (Size i{0}; i < C; i++)
        {
            (*this)(n, i) = mat(0, i);
        }
    }

    /// @brief Overwrite a contiguous subset of row `n`.
    ///
    /// @tparam startIndex  First column to write (inclusive, compile-time).
    /// @tparam endIndex    Last column to write (inclusive, compile-time).
    /// @param  mat         Source 1×(endIndex-startIndex+1) matrix.
    /// @param  n           Zero-based target row index.
    template <Size startIndex, Size endIndex>
    constexpr void setRow(const Matrix<T, 1, endIndex - startIndex + 1> &mat,
                          const Size n)
    {
        static_assert(C > startIndex, "startIndex cannot be larger than array");
        static_assert(C > endIndex, "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex,
                      "startIndex cannot be larger than endIndex");

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            (*this)(n, i) = mat(0, i - startIndex);
        }
    }

    /// @brief Overwrite column `n` with the contents of `mat`.
    /// @param mat  R×1 source matrix.
    /// @param n    Zero-based target column index.
    constexpr void setCol(const Matrix<T, R, 1> &mat, const Size n)
    {
        for (Size j{0}; j < R; j++)
        {
            (*this)(j, n) = mat(j, 0);
        }
    }

    /// @brief Overwrite a contiguous subset of column `n`.
    ///
    /// @tparam startIndex  First row to write (inclusive, compile-time).
    /// @tparam endIndex    Last row to write (inclusive, compile-time).
    /// @param  mat         Source (endIndex-startIndex+1)×1 matrix.
    /// @param  n           Zero-based target column index.
    template <Size startIndex, Size endIndex>
    constexpr void setCol(const Matrix<T, endIndex - startIndex + 1, 1> &mat,
                          const Size n)
    {
        static_assert(R > startIndex, "startIndex cannot be larger than array");
        static_assert(R > endIndex, "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex,
                      "startIndex cannot be larger than endIndex");

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            (*this)(i, n) = mat(i - startIndex, 0);
        }
    }

    /// @brief Overwrite a rectangular subregion of this matrix.
    ///
    /// Copies `mat` into the region `[x1..x2][y1..y2]` (all inclusive,
    /// zero-based). The source matrix must have dimensions
    /// `(y2-y1+1) × (x2-x1+1)`.
    ///
    /// @tparam x1  Top-left row.
    /// @tparam y1  Top-left column.
    /// @tparam x2  Bottom-right row. Must be `>= x1`.
    /// @tparam y2  Bottom-right column. Must be `>= y1`.
    // x1,y1,x2,y2 are indexes
    template <Size x1, Size y1, Size x2, Size y2>
    constexpr void setSub(const Matrix<T, y2 - y1 + 1, x2 - x1 + 1> &mat)
    {
        static_assert(x2 >= x1,
                      "Second x index must be bigger than the first.");
        static_assert(y2 >= y1,
                      "Second y index must be bigger than the first.");

        for (Size i{x1}; i <= x2; i++)
        {
            for (Size j{y1}; j <= y2; j++)
            {
                (*this)(i, j) = mat(i - x1, j - y1);
            }
        }
    }

    /// @brief Returns `true` if the matrix has equal row and column counts.
    constexpr bool isSquare() const
    {
        return sizeX() == sizeY();
    }

    /// @brief Returns `true` if the matrix is symmetric within machine epsilon.
    ///
    /// Uses @ref equalWithin with @ref epsilon<T>() as the tolerance.
    /// For floating-point matrices, prefer the threshold overload to control
    /// the tolerance explicitly.
    ///
    /// @pre `R == C` (enforced by `static_assert`).
    constexpr bool isSymmetric() const
    {
        static_assert(R == C, "Symmetric matrices should be square.");

        if (sizeX() > 1)
        {
            for (Size i{1}; i <= sizeX() - 1; i++)
            {
                for (Size j{0}; j < i; j++)
                {
                    bool eq{false};
                    if constexpr (is_float<T>())
                    {
                        eq = equalWithin((*this)(i, j), (*this)(j, i),
                                         epsilon<T>());
                    }
                    else
                    {
                        eq = ((*this)(i, j) == (*this)(j, i));
                    }

                    if (!eq)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    /// @brief Returns `true` if the matrix is symmetric within `thresh`.
    ///
    /// Checks every off-diagonal pair `(i,j)` / `(j,i)` using
    /// @ref equalWithin. Use this overload when you need explicit control
    /// over the symmetry tolerance (e.g., for the eigenvalue solver routing
    /// threshold @ref CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE).
    ///
    /// @tparam U      Type of the threshold. Must be floating-point.
    /// @param  thresh Absolute tolerance for the symmetry check.
    /// @pre   `R == C` and `T` must be a floating-point type.
    template <typename U> constexpr bool isSymmetric(const U thresh) const
    {
        static_assert(is_float<T>(),
                      "isSymmetric with threshold requires floating-point "
                      "matrix elements; integer T would truncate thresh");
        static_assert(is_float<U>(), "isSymmetric with arg expects to compare\
                floating point values");
        static_assert(R == C, "Symmetric matrices should be square.");

        if (sizeX() > 1)
        {
            for (Size i{1}; i <= sizeX() - 1; i++)
            {
                for (Size j{0}; j < i; j++)
                {
                    if (!equalWithin((*this)(i, j), (*this)(j, i), thresh))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    /// @brief Number of rows (same as template parameter `R`).
    constexpr Size sizeX() const
    {
        return R;
    }
    /// @brief Number of columns (same as template parameter `C`).
    constexpr Size sizeY() const
    {
        return C;
    }

    /// @brief Raw row-major storage. Prefer `operator()` for element access.
    Array<Array<T, C>, R> _data{};
};

/// @}  // defgroup matrix

} // namespace consteig
#endif // MATRIX_HPP

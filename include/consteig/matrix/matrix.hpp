#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../consteig_types.hpp"
#include "../math/math_backend.hpp"

namespace consteig
{

/// @defgroup matrix Matrix
/// @brief Fixed-size matrix type and arithmetic operations.
/// @{

// Forward declarations for member wrapper implementations
template <typename T, Size R, Size C> class Matrix;

template <typename T, Size R, Size C>
constexpr Matrix<T, C, R> transpose(const Matrix<T, R, C> &mat);

template <typename T, Size R, Size C>
constexpr T trace(const Matrix<T, R, C> &mat);

template <typename T, Size R, Size C>
constexpr T determinant(const Matrix<T, R, C> &mat);

template <typename T, Size R, Size C>
constexpr T norm(const Matrix<T, R, C> &mat);

template <typename T, Size R, Size C>
constexpr T dot(const Matrix<T, R, C> &lhs, const Matrix<T, R, C> &rhs);

/// @brief Fixed-size matrix with compile-time dimensions.
///
/// The primary container type for all consteig operations. All member
/// functions are `constexpr`, enabling full compile-time evaluation when
/// the matrix is declared `static constexpr`.
///
/// Storage is row-major: `_data[i][j]` holds row `i`, column `j`.
/// Element access uses `operator()(i, j)` using zero-based indices.
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
    /// @brief Access element at row `row`, column `col` (mutable).
    constexpr T &operator()(const Size row, const Size col)
    {
        return _data[row][col];
    }

    /// @brief Access element at row `row`, column `col` (read-only).
    constexpr const T &operator()(const Size row, const Size col) const
    {
        return _data[row][col];
    }

    /// @brief Exact element-wise equality. Prefer @ref equalWithin(rhs, thresh)
    /// for floats.
    template <typename U>
    constexpr bool operator==(const Matrix<U, R, C> &rhs) const
    {
        for (Size row{0}; row < R; row++)
        {
            for (Size col{0}; col < C; col++)
            {
                if ((*this)(row, col) != rhs(row, col))
                {
                    return false;
                }
            }
        }
        return true;
    }

    /// @brief Inequality (exact). Prefer negated @ref equalWithin(rhs, thresh)
    /// for floats.
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

        for (Size col{0}; col < C; col++)
        {
            result(0, col) = (*this)(n, col);
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

        for (Size col{startIndex}; col <= endIndex; col++)
        {
            result(0, col - startIndex) = (*this)(n, col);
        }

        return result;
    }

    /// @brief Extract column `n` as an R×1 matrix.
    /// @param n  Zero-based column index. Must be `< C`.
    constexpr Matrix<T, R, 1> col(const Size n) const
    {
        Matrix<T, R, 1> result{};

        for (Size row{0}; row < R; row++)
        {
            result(row, 0) = (*this)(row, n);
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

        for (Size row{startIndex}; row <= endIndex; row++)
        {
            result(row - startIndex, 0) = (*this)(row, n);
        }

        return result;
    }

    /// @brief Extract a rectangular submatrix.
    ///
    /// Returns a `numRows × numCols` submatrix starting at `(startRow,
    /// startCol)`. Dimensions are compile-time constants; start position is
    /// runtime.
    ///
    /// @tparam numRows   Number of rows to extract (compile-time).
    /// @tparam numCols   Number of columns to extract (compile-time).
    /// @param  startRow  Top-left row index (zero-based, runtime).
    /// @param  startCol  Top-left column index (zero-based, runtime).
    template <Size numRows, Size numCols>
    constexpr Matrix<T, numRows, numCols> block(Size startRow,
                                                Size startCol) const
    {
        Matrix<T, numRows, numCols> result{};

        for (Size row{startRow}; row < startRow + numRows; row++)
        {
            for (Size col{startCol}; col < startCol + numCols; col++)
            {
                result(row - startRow, col - startCol) = (*this)(row, col);
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
        for (Size col{0}; col < C; col++)
        {
            (*this)(n, col) = mat(0, col);
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

        for (Size col{startIndex}; col <= endIndex; col++)
        {
            (*this)(n, col) = mat(0, col - startIndex);
        }
    }

    /// @brief Overwrite column `n` with the contents of `mat`.
    /// @param mat  R×1 source matrix.
    /// @param n    Zero-based target column index.
    constexpr void setCol(const Matrix<T, R, 1> &mat, const Size n)
    {
        for (Size row{0}; row < R; row++)
        {
            (*this)(row, n) = mat(row, 0);
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

        for (Size row{startIndex}; row <= endIndex; row++)
        {
            (*this)(row, n) = mat(row - startIndex, 0);
        }
    }

    /// @brief Overwrite a rectangular subregion of this matrix.
    ///
    /// Copies the `numRows × numCols` source matrix `mat` into this matrix
    /// starting at `(startRow, startCol)`. Dimensions are compile-time;
    /// start position is runtime.
    ///
    /// @tparam numRows   Number of rows to write (compile-time).
    /// @tparam numCols   Number of columns to write (compile-time).
    /// @param  mat       Source matrix of dimensions `numRows × numCols`.
    /// @param  startRow  Top-left target row (zero-based, runtime).
    /// @param  startCol  Top-left target column (zero-based, runtime).
    template <Size numRows, Size numCols>
    constexpr void setBlock(const Matrix<T, numRows, numCols> &mat,
                            Size startRow, Size startCol)
    {
        for (Size row{startRow}; row < startRow + numRows; row++)
        {
            for (Size col{startCol}; col < startCol + numCols; col++)
            {
                (*this)(row, col) = mat(row - startRow, col - startCol);
            }
        }
    }

    /// @brief Returns `true` if the matrix has equal row and column counts.
    constexpr bool isSquare() const
    {
        return rows() == cols();
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

        if (rows() > 1)
        {
            for (Size row{1}; row <= rows() - 1; row++)
            {
                for (Size col{0}; col < row; col++)
                {
                    bool eq{false};
                    if constexpr (is_float<T>())
                    {
                        eq = consteig::equalWithin(
                            (*this)(row, col), (*this)(col, row), epsilon<T>());
                    }
                    else
                    {
                        eq = ((*this)(row, col) == (*this)(col, row));
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

        if (rows() > 1)
        {
            for (Size row{1}; row <= rows() - 1; row++)
            {
                for (Size col{0}; col < row; col++)
                {
                    if (!consteig::equalWithin((*this)(row, col),
                                               (*this)(col, row), thresh))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    /// @brief Returns `true` if every element of `rhs` is within `thresh` of
    /// the corresponding element of `*this`.
    ///
    /// The free function @ref equalWithinMat delegates to this. Prefer this
    /// over `operator==` for floating-point matrices.
    ///
    /// @param  rhs    Matrix to compare against.
    /// @param  thresh Absolute per-element tolerance.
    constexpr bool equalWithin(const Matrix<T, R, C> &rhs, const T thresh) const
    {
        for (Size row{0}; row < R; row++)
        {
            for (Size col{0}; col < C; col++)
            {
                if (!consteig::equalWithin((*this)(row, col), rhs(row, col),
                                           thresh))
                {
                    return false;
                }
            }
        }
        return true;
    }

    /// @brief Number of rows (same as template parameter `R`).
    constexpr Size rows() const
    {
        return R;
    }
    /// @brief Number of columns (same as template parameter `C`).
    constexpr Size cols() const
    {
        return C;
    }

    /// @brief Raw pointer to first element (mutable).
    constexpr T *data()
    {
        return &_data[0][0];
    }

    /// @brief Raw pointer to first element (read-only).
    constexpr const T *data() const
    {
        return &_data[0][0];
    }

    /// @brief Member convenience wrappers — delegate to free functions.
    /// @{

    /// @brief Returns the transpose. Delegates to @ref consteig::transpose.
    constexpr Matrix<T, C, R> transpose() const
    {
        return consteig::transpose(*this);
    }

    /// @brief Returns the trace. Delegates to @ref consteig::trace.
    constexpr T trace() const
    {
        return consteig::trace(*this);
    }

    /// @brief Returns the determinant. Delegates to @ref consteig::determinant.
    constexpr T determinant() const
    {
        return consteig::determinant(*this);
    }

    /// @brief Returns the Frobenius norm. Delegates to @ref consteig::norm.
    constexpr T norm() const
    {
        return consteig::norm(*this);
    }

    /// @brief Dot product with `other`. Delegates to @ref consteig::dot.
    constexpr T dot(const Matrix<T, R, C> &other) const
    {
        return consteig::dot(*this, other);
    }

    /// @}

    // Public for aggregate initialization only. C++17 aggregates require all
    // data members to be public; making this private breaks the {{...}} syntax.
    // Treat as an implementation detail — use operator() for element access.
    //
    // Row-major storage: _data[row][col]. This differs from Eigen and LAPACK,
    // which default to column-major. The tradeoff is intentional: row-major
    // matches C's native 2D array layout, so aggregate initialization reads
    // naturally as written-out matrix rows (e.g. {{1,2},{3,4}}). Interop via
    // data() with Eigen/LAPACK will produce transposed results unless the
    // consumer specifies row-major explicitly (e.g. Eigen::RowMajor).
    T _data[R][C]{};
};

/// @brief Construct a Matrix from a flat list of scalar arguments in row-major
/// order.
///
/// Alternative to aggregate initialization when nested braces are inconvenient.
/// Arguments are filled row by row, left to right — identical to how values
/// appear when written out as a matrix on paper.
///
/// @tparam T     Scalar element type.
/// @tparam R     Number of rows (compile-time).
/// @tparam C     Number of columns (compile-time).
/// @tparam Args  Deduced scalar argument types; must all be convertible to `T`.
///
/// The number of arguments must equal `R * C` exactly (enforced by
/// `static_assert`).
///
/// @code
/// // 2x3 matrix — equivalent to aggregate init
/// static constexpr auto m = make_matrix<double, 2, 3>(1.0, 2.0, 3.0,
///                                                     4.0, 5.0, 6.0);
/// @endcode
template <typename T, Size R, Size C, typename... Args>
constexpr Matrix<T, R, C> make_matrix(Args... args)
{
    static_assert(sizeof...(Args) == R * C,
                  "make_matrix: argument count must equal R * C");

    Matrix<T, R, C> result{};
    T flat[] = {static_cast<T>(args)...};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result(row, col) = flat[row * C + col];
        }
    }

    return result;
}

/// @brief Convert a matrix from one element type to another.
///
/// Performs an element-wise `static_cast<To>` on each element. This is the
/// preferred way to change element type (e.g. `double` to `float`) since
/// `Matrix` has no converting constructor in order to preserve aggregate
/// initialization.
///
/// @tparam To    Target element type.
/// @tparam From  Source element type.
/// @tparam R     Number of rows (compile-time).
/// @tparam C     Number of columns (compile-time).
/// @param  src   Input matrix with element type `From`.
/// @return New matrix with element type `To` and the same dimensions.
///
/// @code
/// static constexpr auto fmat = consteig::matrix_cast<float>(dmat);
/// @endcode
template <typename To, typename From, Size R, Size C>
constexpr Matrix<To, R, C> matrix_cast(const Matrix<From, R, C> &src)
{
    Matrix<To, R, C> result{};

    for (Size row{0}; row < R; row++)
    {
        for (Size col{0}; col < C; col++)
        {
            result(row, col) = static_cast<To>(src(row, col));
        }
    }

    return result;
}

/// @}  // defgroup matrix

} // namespace consteig
#endif // MATRIX_HPP

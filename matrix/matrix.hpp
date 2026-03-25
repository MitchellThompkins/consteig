#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../consteig_types.hpp"
#include "../math/functions/utilities.hpp"

namespace consteig
{

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

template <typename T, Size R, Size C> class Matrix
{
  public:
    constexpr T &operator()(const Size row, const Size col)
    {
        return _data[row][col];
    }

    constexpr const T &operator()(const Size row, const Size col) const
    {
        return _data[row][col];
    }

    // TODO(mthompkins): Need to handle the equal floats case
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

    template <typename U>
    constexpr bool operator!=(const Matrix<U, R, C> &rhs) const
    {
        return !(*this == rhs);
    }

    constexpr Matrix<T, 1, C> row(const Size n) const
    {
        Matrix<T, 1, C> result{};

        for (Size col{0}; col < C; col++)
        {
            result(0, col) = (*this)(n, col);
        }

        return result;
    }

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

    constexpr Matrix<T, R, 1> col(const Size n) const
    {
        Matrix<T, R, 1> result{};

        for (Size row{0}; row < R; row++)
        {
            result(row, 0) = (*this)(row, n);
        }

        return result;
    }

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

    constexpr void setRow(const Matrix<T, 1, C> &mat, const Size n)
    {
        for (Size col{0}; col < C; col++)
        {
            (*this)(n, col) = mat(0, col);
        }
    }

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

    constexpr void setCol(const Matrix<T, R, 1> &mat, const Size n)
    {
        for (Size row{0}; row < R; row++)
        {
            (*this)(row, n) = mat(row, 0);
        }
    }

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

    constexpr bool isSquare() const
    {
        return rows() == cols();
    }

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
                        eq = equalWithin((*this)(row, col), (*this)(col, row),
                                         epsilon<T>());
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
                    if (!equalWithin((*this)(row, col), (*this)(col, row),
                                     thresh))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    constexpr Size rows() const
    {
        return R;
    }
    constexpr Size cols() const
    {
        return C;
    }

    constexpr T *data()
    {
        return &_data[0][0];
    }

    constexpr const T *data() const
    {
        return &_data[0][0];
    }

    // Member wrappers delegating to free functions in operations.hpp
    constexpr Matrix<T, C, R> transpose() const
    {
        return consteig::transpose(*this);
    }

    constexpr T trace() const
    {
        return consteig::trace(*this);
    }

    constexpr T determinant() const
    {
        return consteig::determinant(*this);
    }

    constexpr T norm() const
    {
        return consteig::norm(*this);
    }

    constexpr T dot(const Matrix<T, R, C> &other) const
    {
        return consteig::dot(*this, other);
    }

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

} // namespace consteig
#endif // MATRIX_HPP

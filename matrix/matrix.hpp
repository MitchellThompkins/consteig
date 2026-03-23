#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../array/array.hpp"
#include "../math/functions/utilities.hpp"

namespace consteig
{

template <typename T, Size R, Size C> class Matrix
{
  public:
    constexpr T &operator()(const Size i, const Size j)
    {
        return _data[i][j];
    }

    constexpr const T &operator()(const Size i, const Size j) const
    {
        return _data[i][j];
    }

    // TODO(mthompkins): Need to handle the equal floats case
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

    template <typename U>
    constexpr bool operator!=(const Matrix<U, R, C> &rhs) const
    {
        return !(*this == rhs);
    }

    constexpr Matrix<T, 1, C> row(const Size n) const
    {
        Matrix<T, 1, C> result{};

        for (Size j{0}; j < C; j++)
        {
            result(0, j) = (*this)(n, j);
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

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            result(0, i - startIndex) = (*this)(n, i);
        }

        return result;
    }

    constexpr Matrix<T, R, 1> col(const Size n) const
    {
        Matrix<T, R, 1> result{};

        for (Size i{0}; i < R; i++)
        {
            result(i, 0) = (*this)(i, n);
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

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            result(i - startIndex, 0) = (*this)(i, n);
        }

        return result;
    }

    template <Size startRow, Size startCol, Size numRows, Size numCols>
    constexpr Matrix<T, numRows, numCols> block() const
    {
        Matrix<T, numRows, numCols> result{};

        for (Size i{startRow}; i < startRow + numRows; i++)
        {
            for (Size j{startCol}; j < startCol + numCols; j++)
            {
                result(i - startRow, j - startCol) = (*this)(i, j);
            }
        }

        return result;
    }

    constexpr Matrix() = default;
    constexpr Matrix(const Array<Array<T, C>, R> &data) : _data(data)
    {
    }
    constexpr Matrix(const Matrix &) = default;
    constexpr Matrix(Matrix &&) = default;
    constexpr Matrix &operator=(const Matrix &) = default;
    constexpr Matrix &operator=(Matrix &&) = default;

    constexpr void setRow(const Matrix<T, 1, C> &mat, const Size n)
    {
        for (Size i{0}; i < C; i++)
        {
            (*this)(n, i) = mat(0, i);
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

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            (*this)(n, i) = mat(0, i - startIndex);
        }
    }

    constexpr void setCol(const Matrix<T, R, 1> &mat, const Size n)
    {
        for (Size j{0}; j < R; j++)
        {
            (*this)(j, n) = mat(j, 0);
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

        for (Size i{startIndex}; i <= endIndex; i++)
        {
            (*this)(i, n) = mat(i - startIndex, 0);
        }
    }

    template <Size startRow, Size startCol, Size numRows, Size numCols>
    constexpr void setBlock(const Matrix<T, numRows, numCols> &mat)
    {
        for (Size i{startRow}; i < startRow + numRows; i++)
        {
            for (Size j{startCol}; j < startCol + numCols; j++)
            {
                (*this)(i, j) = mat(i - startRow, j - startCol);
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
            for (Size i{1}; i <= rows() - 1; i++)
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
            for (Size i{1}; i <= rows() - 1; i++)
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

  private:
    Array<Array<T, C>, R> _data{};
};

} // namespace consteig
#endif // MATRIX_HPP

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "stddef.h"
#include "array.hpp"

template< typename T, size_t C, size_t R >
class Matrix
{
public:
    //TODO(mthompkins): Fix neccessity to declare add () for an empty matrix.
    //constexpr Matrix(Array< Array<T, C>, R> m) : _data(m)
    //{}

    constexpr T& operator()(const size_t i, const size_t j)
    {
        return _data[i][j];
    }
    constexpr const T& operator()(const size_t i, const size_t j) const
    {
        return _data[i][j];
    }

    template <typename U>
    constexpr bool operator==(const Matrix<U, R, C> &rhs) const
    {
        for (size_t i = 0; i < R; i++)
            for (size_t j = 0; j < C; j++)
                if ((*this)(i, j) != rhs(i, j))
                    return false;
        return true;
    }

    template <typename U>
    constexpr bool operator!=(const Matrix<U, R, C> &rhs) const
    {
        return !(*this == rhs);
    }

    Array< Array<T, C>, R> _data{};
};

template< typename T, size_t C, size_t R >
constexpr Matrix<T,C,R> operator+(
        const Matrix<T,C,R> &lhs,
        const Matrix<T,C,R> &rhs )
{
    Matrix<T,C,R> result{};

    for (std::size_t i = 0; i < R; ++i)
        for (std::size_t j = 0; j < C; ++j)
            result(i, j) = lhs(i, j) + rhs(i, j);

    return result;
}

#endif // MATRIX_HPP

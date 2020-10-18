#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "stddef.h"
#include "array.hpp"

template< typename T, size_t R, size_t C >
class Matrix
{
public:
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

template< typename T, size_t R, size_t C >
constexpr Matrix<T,R,C> operator+(
        const Matrix<T,R,C> &lhs,
        const Matrix<T,R,C> &rhs )
{
    Matrix<T,R,C> result{};

    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            result(i, j) = lhs(i, j) + rhs(i, j);

    return result;
}

template< typename T, size_t R, size_t C >
constexpr Matrix<T,R,C> operator-(
        const Matrix<T,R,C> &lhs,
        const Matrix<T,R,C> &rhs )
{
    Matrix<T,R,C> result{};

    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            result(i, j) = lhs(i, j) - rhs(i, j);

    return result;
}

template< typename T, size_t R, size_t C >
constexpr Matrix<T,R,C> operator*(
        const Matrix<T,R,C> &lhs,
        const Matrix<T,R,C> &rhs )
{
    Matrix<T,R,C> result{};

    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < C; ++j)
            result(i, j) = lhs(i, j) + rhs(i, j);

    return result;
}

#endif // MATRIX_HPP

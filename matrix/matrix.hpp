#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "stddef.h"
#include "array.hpp"
#include "gcem.hpp"

template<typename T, size_t R, size_t C>
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
        for (size_t i {0}; i < R; i++)
            for (size_t j {0}; j < C; j++)
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

template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,C> operator+(
        const Matrix<T,R,C> &lhs,
        const Matrix<T,R,C> &rhs )
{
    Matrix<T,R,C> result{};

    for (size_t i {0}; i < R; ++i)
        for (size_t j {0}; j < C; ++j)
            result(i, j) = lhs(i, j) + rhs(i, j);

    return result;
}

template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,C> operator-(
        const Matrix<T,R,C> &lhs,
        const Matrix<T,R,C> &rhs )
{
    Matrix<T,R,C> result{};

    for (size_t i {0}; i < R; ++i)
        for (size_t j {0}; j < C; ++j)
            result(i, j) = lhs(i, j) - rhs(i, j);

    return result;
}

template<typename T, size_t R1, size_t C1, size_t R2, size_t C2>
constexpr Matrix<T,R1,C2> operator*(
        const Matrix<T,R1,C1> &lhs,
        const Matrix<T,R2,C2> &rhs )
{
    static_assert(C1==R2, "Number of columns must equal number of rows");
    Matrix<T,R1,C2> result{};

    for(size_t i {0}; i<R1; i++)
        for(size_t j {0}; j<C2; j++)
            for(size_t k {0}; k<C1; k++)
                    result(i,j) += lhs(i,k)*rhs(k,j);

    return result;
}

// todo(mthompkins): Figure out how to not make it possible to pass the scalar
// to either side
template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,C> operator*(
        const T &lhs,
        const Matrix<T,R,C> &rhs )
{
    Matrix<T,R,C> result{};

    for(size_t i {0}; i<R; i++)
        for(size_t j {0}; j<C; j++)
            result(i,j) = lhs*rhs(i,j);

    return result;
}

template<typename T, size_t R, size_t C>
constexpr Matrix<T,C,R> transpose(
        const Matrix<T,R,C> &mat )
{
    Matrix<T,C,R> result{};

    for(size_t i {0}; i<R; i++)
        for(size_t j {0}; j<C; j++)
            result(j,i) = mat(i,j);

    return result;
}

template<typename T, size_t X>
constexpr Matrix<T,X,X> diagional( const T val )
{
    Matrix<T,X,X> result{};

    for(size_t i{0}, j{0}; i<X; i++,j++ )
        result(i,j) = val;

    return result;
}

template<typename T, size_t R, size_t C>
constexpr T normE( const Matrix<T,R,C> &mat )
{
    T result{};

    for(size_t i {0}; i<R; i++)
        for(size_t j {0}; j<C; j++)
            result += (mat(i,j)*mat(i,j));

    return gcem::sqrt(result);

}

#endif // MATRIX_HPP

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "stddef.h"
#include "array.hpp"
#include "gcem.hpp"

template< typename T, size_t C, size_t R >
class Matrix
{
public:
    //TODO(mthompkins): Fix neccessity to declare add () for an empty matrix.
    constexpr Matrix(Array< Array<T, C>, R> m) : _data(m)
    {}

    constexpr T& operator()(const size_t i, const size_t j)
    {
        return _data[i][j];
    }
    constexpr const T& operator()(const size_t i, const size_t j) const
    {
        return _data[i][j];
    }
private:

    Array< Array<T, C>, R> _data;
};

#endif // MATRIX_HPP

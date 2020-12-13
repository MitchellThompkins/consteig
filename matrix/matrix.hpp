#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "stddef.h"

#include "../array/array.hpp"
#include "matrix_tools.hpp"

namespace consteig
{

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

    //TODO(mthompkins): Need to handle the equal floats case
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

    constexpr Matrix<T,1,C> row(const size_t n) const
    {
        Matrix<T,1,C> result{};

        for( size_t j {0}; j<C; j++ )
            result(0,j) = (*this)(n,j);

        return result;
    }

    // Get subset of row
    template<size_t startIndex, size_t endIndex>
    constexpr Matrix<T,1,endIndex-startIndex+1> row(const size_t n) const
    {
        static_assert(C > startIndex, "startIndex cannot be larger than array");
        static_assert(C > endIndex,   "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex, "startIndex cannot be larger than endIndex");

        Matrix<T,1,endIndex-startIndex+1> result{};

        for( size_t i {startIndex}; i<=endIndex; i++ )
            result(0,i-startIndex) = (*this)(n,i);

        return result;
    }

    constexpr Matrix<T,R,1> col(const size_t n) const
    {
        Matrix<T,R,1> result{};

        for( size_t i {0}; i<C; i++ )
            result(i,0) = (*this)(i,n);

        return result;
    }

    // Get subset of column
    template<size_t startIndex, size_t endIndex>
    constexpr Matrix<T,endIndex-startIndex+1,1> col(const size_t n) const
    {
        static_assert(R > startIndex, "startIndex cannot be larger than array");
        static_assert(R > endIndex,   "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex, "startIndex cannot be larger than endIndex");

        Matrix<T,endIndex-startIndex+1,1> result{};

        for( size_t i {startIndex}; i<=endIndex; i++ )
            result(i-startIndex,0) = (*this)(i,n);

        return result;
    }

    // x1,y1,x2,y2 are indexes
    template<size_t x1, size_t y1, size_t x2, size_t y2>
    constexpr Matrix<T, x2-x1+1, y2-y1+1> sub() const
    {
        static_assert(x2>=x1, "Second x index must be bigger than the first.");
        static_assert(y2>=y1, "Second y index must be bigger than the first.");

        Matrix<T, x2-x1+1, y2-y1+1> result{};

        for( size_t i {x1}; i<=x2; i++ )
            for( size_t j {y1}; j<=y2; j++ )
                result(i-x1,j-y1) = (*this)(i,j);

        return result;
    }

    //todo(mthompkins): Add test for this
    constexpr void operator=(const Matrix<T, R, C> &rhs)
    {
        for( size_t i{0}; i<R; i++ )
            for( size_t j{0}; j<C; j++ )
                (*this)(i,j) = rhs(i,j);
    }

    constexpr void setRow(const Matrix<T,1,C> &mat, const size_t n)
    {
        for( size_t i {0}; i<C; i++ )
            (*this)(n,i) = mat(0,i);
    }

    template<size_t startIndex, size_t endIndex>
    constexpr void setRow(
            const Matrix<T,1,endIndex-startIndex+1> &mat,
            const size_t n )
    {
        static_assert(C > startIndex, "startIndex cannot be larger than array");
        static_assert(C > endIndex,   "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex, "startIndex cannot be larger than endIndex");

        for( size_t i {startIndex}; i<=endIndex; i++ )
            (*this)(n,i) = mat(0,i-startIndex);
    }

    constexpr void setCol(
            const Matrix<T,R,1> &mat,
            const size_t n)
    {
        for( size_t j {0}; j<R; j++ )
            (*this)(j,n) = mat(j,0);
    }

    template<size_t startIndex, size_t endIndex>
    constexpr void setCol(
            const Matrix<T,endIndex-startIndex+1,1> &mat,
            const size_t n )
    {
        static_assert(R > startIndex, "startIndex cannot be larger than array");
        static_assert(R > endIndex,   "endIndex cannot be larger than array");
        static_assert(endIndex >= startIndex, "startIndex cannot be larger than endIndex");

        for( size_t i {startIndex}; i<=endIndex; i++ )
            (*this)(i,n) = mat(i-startIndex,0);
    }

    // x1,y1,x2,y2 are indexes
    template<size_t x1, size_t y1, size_t x2, size_t y2>
    constexpr void setSub(
            const Matrix<T, y2-y1+1, x2-x1+1> &mat )
    {
        static_assert(x2>=x1, "Second x index must be bigger than the first.");
        static_assert(y2>=y1, "Second y index must be bigger than the first.");

        for( size_t i {x1}; i<=x2; i++ )
            for( size_t j {y1}; j<=y2; j++ )
                (*this)(i,j) = mat(i-x1,j-y1);
    }

    constexpr bool isSquare() const
    {
        return sizeX()==sizeY();
    }

    constexpr bool isSymmetric() const
    {
        static_assert(R==C, "Symmetric matrices should be square.");

        bool symmetric {true};

        if(sizeX()>1)
        {
            for( unsigned int i {1}; i<=sizeX()-1; i++ )
            {
                for( unsigned int j {0}; j<i; j++ )
                {
                    symmetric &= ((*this)(i,j) == (*this)(j,i));
                    if(!symmetric)
                        break;
                }
            }
        }

        return symmetric;
    }

    template<typename U>
    constexpr bool isSymmetric( const U thresh ) const
    {
        static_assert( is_float<U>(), "isSymmetric with arg expects to compare\
                floating point values");
        static_assert(R==C, "Symmetric matrices should be square.");

        bool symmetric {true};

        if(sizeX()>1)
        {
            for( unsigned int i {1}; i<=sizeX()-1; i++ )
            {
                for( unsigned int j {0}; j<i; j++ )
                {
                    symmetric &= compareFloats((*this)(i,j), (*this)(j,i), thresh);
                    if(!symmetric)
                        break;
                }
            }
        }

        return symmetric;
    }

    constexpr size_t sizeX() const { return R; }
    constexpr size_t sizeY() const { return C; }

    Array< Array<T, C>, R> _data{};
};

}
#endif // MATRIX_HPP

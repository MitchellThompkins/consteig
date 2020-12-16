#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "matrix.hpp"
#include "../dependencies/gcem/include/gcem.hpp"

namespace consteig
{

/*
 * | Operation             | Operator    |
 * |-----------------------+-------------|
 * | Addition              | +           |
 * | Subtraction           | -           |
 * | Matrix Multiplication | *           |
 * | Scalar Multiplication | *           |
 * | Dot Product           | dot()       |
 * | Transpose             | transpose() |
 * | Create Diagonal       | diagonal()  |
 * | Create Identity       | eye()       |
 * | Euclidean Normal      | normE()     |
 * | Square Root           | sqrt()      |
 * | Determinant           | det()       |
*/


// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf
///////////// IMPLEMENTATIONS /////////////

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

//Multiply two matrices
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

// Multiply by a scalar
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

// Multiply a 1XN by a Nx1 matrix
template<typename T, size_t R, size_t C>
constexpr T dot(
        const Matrix<T,R,C> &lhs,
        const Matrix<T,R,C> &rhs )
{
    static_assert( R==1, "Dot Product expects two 1xN matrices");
    Matrix<T,1,1> product {lhs*transpose(rhs)};
    T result {product(0,0)};

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

template<typename T, size_t S>
constexpr Matrix<T,S,S> diagonal( const T val )
{
    Matrix<T,S,S> result{};

    for(size_t i{0}, j{0}; i<S; i++,j++ )
        result(i,j) = val;

    return result;
}

template<typename T, size_t S>
constexpr Matrix<T,S,S> eye()
{
    return diagonal<T,S>(static_cast<T>(1));
}

// Euclidean normal of a matrix
template<typename T, size_t R, size_t C>
constexpr T normE( const Matrix<T,R,C> &mat )
{
    T result{};

    for(size_t i {0}; i<R; i++)
        for(size_t j {0}; j<C; j++)
            result += (mat(i,j)*mat(i,j));

    return consteig::sqrt(result);
}

template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,C> sqrt( const Matrix<T,R,C> &mat )
{
    T result{};

    for(size_t i {0}; i<R; i++)
        for(size_t j {0}; j<C; j++)
            result(i,j) = consteig::sqrt(mat(i,j));

    return result;
}

template<typename T, size_t R, size_t C>
constexpr T det( const Matrix<T,R,C> &mat )
{
    static_assert(R==C, "Can only find determinant of a square matrix");

    Matrix<T,R-1,C-1> submat{};

    T result {static_cast<T>(0)};

    if(R==1)
    {
        result = mat(0,0);
    }
    else if(R==2)
    {
        result = det(mat);
    }
    else
    {
        for(size_t i {0}; i<R; i++)
        {
            size_t subi {0U};
            for(size_t j {1}; j<R; j++)
            {
                size_t subj {0U};
                for(size_t k {0}; k<R; k++)
                {
                    if(k==i)
                    {
                        continue;
                    }
                    submat(subi,subj) = mat(j,k);
                    subj++;
                }
                subi++;
            }
            //TODO(mthompkins): Figure out how to handle unsigned T for pow
            result += (gcem::pow<T>(-1, i) * mat(0,i) * det(submat));
        }
    }

    return result;
}

template<typename T>
constexpr T det( const Matrix<T,2,2> &mat )
{
    return (mat(0,0)*mat(1,1)) - (mat(0,1)*mat(1,0));
}

}
#endif

#ifndef HESSENBERG_HPP
#define HESSENBERG_HPP

#include "stddef.h"

#include "householder.hpp"

#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

/*
 * The implementation here warrants an explantation. The hessenberg reduction
 * returns two matrices, a P and an H. P is a unitary matrix and H is "nearly"
 * an upper triangular matrix. Most implementations are iterative, but as
 * iterative implementations in constexpr functions are difficult or impossible,
 * that leaves recursion. The hessenberg implementation requires the original
 * matrix A to be modified at each step, stepping down through the matrix
 * dimensions. This proves to be a problem for a recursive implementation
 * because the matrix which gets modified must be continuously decreasing in
 * its dimension, but if it is required to multiply by A at each step, A _can't_
 * get smaller. This is why L is passed in as a template parameter, so that the
 * A continuously sized A matrix can be passed at each recursive step all the
 * while the sub matrix by which it's multiplied can decrease in size.
 *
 * Of course, recursive implementations in templated functions must be provided
 * an explicit specialization to end the recursion. Unfortunately because we
 * know that recursion should end with the matrix by which we are going to
 * multiply is of size 2x2, but b/c A isn't changing in size, it forces us to
 * require a partially specialized templated function, which isn't possible.
 * Instead, we use the trick here:
 * https://www.fluentcpp.com/2017/08/11/how-to-do-partial-template-specialization-in-c/
 *
 * Partial specialization of structs _is_ allowed, so we exploit that fact and
 * have a function return a struct, where the struct is nothing more than the
 * specialized implementation.
 */

///////////// TYPES /////////////
template<typename T, size_t S>
struct PHMatrix
{
    Matrix<T,S,S> _p;
    Matrix<T,S,S> _h;

    constexpr void operator=(const PHMatrix<T, S> &rhs)
    {
        for( size_t i{0}; i<S; i++ )
            for( size_t j{0}; j<S; j++ )
            {
                (*this._p)(i,j) = rhs._p(i,j);
                (*this._h)(i,j) = rhs._h(i,j);
            }
    }
};

///////////// FUNCTION DECLARATIONS /////////////
template<typename T, size_t R, size_t C, size_t L=R>
constexpr PHMatrix<T,R> hess(Matrix<T,R,C> a);


///////////// IMPLEMENTATIONS /////////////
template<typename T, size_t R, size_t C, size_t L=R>
struct hess_impl
{
    static constexpr PHMatrix<T,R> _(Matrix<T,R,C> a)
    {
        static_assert( R==C, "Hessenberg reduction expects a square matrix");

        constexpr size_t size {R};
        constexpr size_t houseSize {L};
        constexpr size_t end {R-1};

        Matrix<T,L,L> subA
        {
            a.template sub<
                R-houseSize,
                R-houseSize,
                end,
                end>()
        };
        Matrix<T,L,L> m {house(subA)};

        Matrix<T,size,size> p {eye<T,R>()};
        p.template setSub<
            R-houseSize+1,
            R-houseSize+1,
            end,
            end>(m.template sub<1,1,houseSize-1,houseSize-1>());

        PHMatrix<T,R> out = hess<T,R,R,houseSize-1>(p*a*p);

        Matrix<T,size,size> pRtn { (houseSize>3) ? p*out._p : p };

        return
        {
            ._p = pRtn,
            ._h = out._h
        };
    }
};

template<typename T, size_t R, size_t C>
struct hess_impl<T, R, C, 2>
{
    static constexpr PHMatrix<T,R> _(Matrix<T,R,C> a)
    {
        return
        {
            ._p = {0},
            ._h = a
        };
    }
};

template<typename T, size_t R, size_t C, size_t L=R>
constexpr PHMatrix<T,R> hess(Matrix<T,R,C> a)
{
    static_assert( is_float<T>(), "hess expects floating point");
    return hess_impl<T,R,C,L>::_(a);
};

} //end namespace
#endif

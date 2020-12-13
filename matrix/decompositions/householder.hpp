#ifndef HOUSEHOLDER_HPP
#define HOUSEHOLDER_HPP

#include "stddef.h"

#include "../matrix_tools.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

///////////// FUNCTION DECLARATIONS /////////////
template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,R> house(Matrix<T,R,C> a);

// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf
///////////// IMPLEMENTATIONS /////////////
template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,R> house(Matrix<T,R,C> a)
{
    static_assert( R==C, "Householder expects a square matrix");
    static_assert( is_float<T>(), "Householder Reflection expects floating point");

    T alphaSum {0};
    for(size_t i {1}; i<R; i++)
        alphaSum += (a(i,0)*a(i,0));

    T alpha { static_cast<T>(-1)
        * gcem::sgn(a(1,0))
        * gcem::sqrt(alphaSum) };

    T r { gcem::sqrt(
            static_cast<T>(0.5)
            * ((alpha*alpha) - (a(1,0)*alpha)) ) };

    T oneOverTwoR {1/(static_cast<T>(2)*r)};

    Matrix<T,R,1> v {static_cast<T>(0)};
    v(1,0) = (a(1,0) - alpha) * oneOverTwoR;
    for(size_t i {2}; i<R; i++)
        v(i,0) = a(i,0) * oneOverTwoR;

    Matrix<T,R,R> p { eye<T,R>() -
        (static_cast<T>(2) * v * transpose(v)) };

    return p;
}

template<typename T>
constexpr Matrix<T,2,2> house(Matrix<T,2,2> a)
{
    Matrix<T,2,2> i { eye<T,2>() };
    i(1,1) = i(1,1)*static_cast<T>(-1);
    return i;
}

}
#endif

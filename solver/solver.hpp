#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "stddef.h"
#include "qr_decomp.hpp"

template<typename T, size_t S>
struct UtMatrix
{
    Matrix<T,S,S> _u;
    Matrix<T,S,S> _t;
};

// https://www.math.kth.se/na/SF2524/matber15/qrmethod.pdf
template<typename T, size_t R, size_t C>
constexpr UtMatrix<T, R> eigensolver( const Matrix<T,R,C> input )
{
    //TODO(mthompkins): Remove this necessity
    static_assert( R==C, "Eigenvalue Solver expects a square matrix");

    Matrix<T,R,R> identity {diagional<T,R>(static_cast<T>(1U))};

    Matrix<T,R,R> a {input};
    Matrix<T,R,R> u[2] = {0};
    u[0] = {identity};

    //TODO(mthompkins): Instead of running for a fixed time, continuously check
    //for an upper triangular matrix
    for( size_t i {1}; i<200; i++ )
    {
        QrMatrix<T, R> aq {qrDecomp(a)};
        a = aq._r*aq._q;
        u[1] = u[0]*aq._q;
        u[0] = u[1];
    }

    return { a, u[0] };

}

template<typename T, size_t R, size_t C>
constexpr Matrix<T, R, 1> eigenvalues( const Matrix<T,R,C> input )
{
    UtMatrix<T,R> ut {eigensolver(input)};

    Matrix<T,R,1> result {};

    for( size_t i {0}; i<R; i++ )
        result(i,0) = ut._u(i,i);

    return result;
}

#endif


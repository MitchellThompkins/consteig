#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "stddef.h"
#include "matrix.hpp"

template<typename T, size_t S>
struct QrMatrix
{
    Matrix<T,S,S> _q;
    Matrix<T,S,S> _r;
};

template<typename T, size_t S>
struct UtMatrix
{
    Matrix<T,S,S> _u;
    Matrix<T,S,S> _t;
};

template<typename T, size_t R, size_t C>
constexpr QrMatrix<T, C> qrDecomp( const Matrix<T,R,C> x )
{
    //TODO(mthompkins): Remove this necessity
    static_assert( R==C, "QR decomposition must be a square matrix");

    QrMatrix<T,C> result {static_cast<T>(0)};
    Matrix<T,C,C> &q {result._q};
    Matrix<T,C,C> &r {result._r};

    // Solve q & R by modified gram-schmidt process
    // https://www.math.ucla.edu/~yanovsky/Teaching/Math151B/handouts/GramSchmidt.pdf
    // https://www.math.ucla.edu/~yanovsky/Teaching/Math151B/handouts/gsqr.m
    // https://www.math.uci.edu/~ttrogdon/105A/html/Lecture23.html

    for(size_t i {0}; i<C; i++)
    {
        q.setCol(x.col(i), i);

        for(size_t j {0}; j<i; j++)
        {
            r(j,i) = dot(transpose(q.col(j)), transpose(q.col(i)));
            Matrix<T,C,1> step {q.col(i) - (r(j,i)*q.col(j))};
            q.setCol(step, i);
        }
        r(i,i) = normE(q.col(i));

        // This is to handle a divide by zero below
        if(r(i,i) != static_cast<T>(0))
            q.setCol( (static_cast<T>(1)/r(i,i)) * q.col(i), i  );
    }

    return result;
}

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

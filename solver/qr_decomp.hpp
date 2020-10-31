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
constexpr QrMatrix<T, C> qrDecomp( const Matrix<T,R,C> input )
{
    //TODO(tthompkins): Remove this necessity
    static_assert( R==C, "QR decomposition must be a square matrix");

    QrMatrix<T,C> result{};

    Matrix<T,1,C> a[C] {};
    Matrix<T,1,C> e[C] {};

    // Solve q by gram-schmidt process
    // (https://www.math.ucla.edu/~yanovsky/Teaching/Math151B/handouts/GramSchmidt.pdf)

    // a[0] = u[0]
    a[0] = transpose(input.col(0));
    e[0] = (1/normE(a[0])) * a[0];

    for(size_t i {1}; i<C; i++)
    {
        a[i] = transpose(input.col(i));
        Matrix<T,1,C> u {a[i]};

        for(size_t j {1}; j<=i; j++)
            u = u - ((dot(a[i],e[j-1]))*e[j-1]);

        e[i] = (1/normE(u)) * u;
    }

    // Create Q Matrix
    for(size_t i {0}; i<C; i++)
        result._q.setCol(transpose(e[i]),i);

    // Create R Matrix
    for(size_t i {0}; i<C; i++)
        for(size_t j {i}; j<C; j++)
            result._r(i,j) = dot(a[j],e[i]);

    return result;
}

template<typename T, size_t R, size_t C>
constexpr UtMatrix<T, R> eigensolver( const Matrix<T,R,C> input )
{
    //TODO(tthompkins): Remove this necessity
    static_assert( R==C, "Eigenvalue Solver expects a square matrix");

    Matrix<T,R,R> identity {diagional<T,R>(static_cast<T>(1U))};

    Matrix<T,R,R> a {input};
    Matrix<T,R,R> u[2] = {0};
    u[0] = {identity};

    for( size_t i {1}; i<500; i++ )
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

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

//template<typename T, size_t S>
template<typename T, size_t R, size_t C>
constexpr QrMatrix<T, R> qrDecomp( const Matrix<T,R,C> mat )
{
    //TODO(tthompkins): Remove this necessity
    static_assert( R==C, "QR decomposition must be a square matrix");

    QrMatrix<T,R> result{};

    Matrix<T,1,R> a[R];
    Matrix<T,1,R> u[R];
    Matrix<T,1,R> e[R];

    //Solve q by gram-schmidt process
    // a[0] = u[0]
    a[0] = mat.row(0);
    e[0] = (1/normE(a[0])) * a[0];

    for(size_t i {1}; i<R; i++)
    {
        Matrix<T,1,R> a[i] {mat.row(i)};

        Matrix<T,1,R> temp {};
        for( size_t j {0}; j<i; j++ )
             temp = temp - dot(a[i],e[i-1])*e[i-1];

        u[i] = a[i] - temp;

        e[i] = (1/normE(a[i])) * a[i];
    }

    //Matrix<T,1,R> a2 {mat.row(1)};
    //Matrix<T,1,R> u2 {a2 - dot(a2,e1)*e1};
    //Matrix<T,1,R> e2 {(1/normE(u2)) * u2};

    //Matrix<T,1,R> a3 {mat.row(2)};
    //Matrix<T,1,R> u3 {a3 - dot(a3,e1)*e1 - dot(a3,e2)*e2};
    //Matrix<T,1,R> e3 {(1/normE(u3)) * u3};

    for(size_t i {0}; i<R; i++)
    {
        result._q.setCol(transpose(e[i]),i);
    }

    return result;
}

#endif

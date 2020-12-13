#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "stddef.h"

#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

template<typename T, size_t S>
struct QRMatrix
{
    Matrix<T,S,S> _q;
    Matrix<T,S,S> _r;
};

template<typename T, size_t R, size_t C>
constexpr QRMatrix<T, C> qr( const Matrix<T,R,C> a )
{
    //TODO(mthompkins): Remove this necessity
    static_assert( R==C, "QR decomposition must be a square matrix");

    QRMatrix<T,R> result {static_cast<T>(0)};
    Matrix<T,R,R> &q {result._q};
    Matrix<T,R,R> &r {result._r};

    // Solve Q & R by modified gram-schmidt process
    // https://www.math.ucla.edu/~yanovsky/Teaching/Math151B/handouts/GramSchmidt.pdf
    // https://www.math.ucla.edu/~yanovsky/Teaching/Math151B/handouts/gsqr.m
    // https://www.math.uci.edu/~ttrogdon/105A/html/Lecture23.html
    
    Matrix<T,R,R> v {a};

    for(size_t i {0}; i<R; i++)
    {
        r(i,i) = normE(v.col(i));

        //TODO(mthompkins): Ensure that this is the right thing to be doing....
        if( r(i,i) != static_cast<T>(0))
        {
            Matrix<T,R,1> setQCol { (1/r(i,i)) * v.col(i)};
            q.setCol(setQCol, i);
        }

        for(size_t j {i+1}; j<R; j++)
        {
            r(i,j) = dot( transpose(q.col(i)), transpose(v.col(j)) );
            Matrix<T,R,1> setVCol { v.col(j) - (r(i,j)*q.col(i)) };
            v.setCol(setVCol, j);
        }
    }

    return result;
}

} //end namespace
#endif

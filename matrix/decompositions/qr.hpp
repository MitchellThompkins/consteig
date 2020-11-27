#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "stddef.h"

#include "../matrix.hpp"
#include "../operations.hpp"

namespace constmat
{

template<typename T, size_t S>
struct QrMatrix
{
    Matrix<T,S,S> _q;
    Matrix<T,S,S> _r;
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

}
#endif

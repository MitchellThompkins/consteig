#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "stddef.h"
#include "matrix.hpp"
#include "qr_decomp.hpp"

template<typename T, size_t S>
struct UTMatrix
{
    Matrix<T,S,S> _u;
    Matrix<T,S,S> _t;
};

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

// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf
template<typename T, size_t R, size_t C>
constexpr Matrix<T,R,R> house(Matrix<T,R,C> a)
{
    static_assert( R==C, "Householder expects a square matrix");

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
    return a;
}

template<typename T, size_t R, size_t C, size_t L>
struct hess_impl
{
    static constexpr Matrix<T,R,R> _(Matrix<T,R,C> a)
    {
        static_assert( R==C, "Hessenberg reduction expects a square matrix");

        constexpr size_t size {R};
        constexpr size_t end {R-1};

        Matrix<T,L,L> subA {a.template sub<R-L,R-L,end,end>()};
        Matrix<T,L,L> m {house(subA)};

        Matrix<T,size,size> p {eye<T,R>()};
        p.template setSub<R-L+1,R-L+1,end,end>(m.template sub<1,1,L-1,L-1>());

        Matrix<T,size,size> temp {p*a*p};
        //Matrix<T,size,size> out = hess<T,R,R,L-1>(temp);

        return a;
        //return out;
    }
};

template<typename T, size_t R, size_t C>
struct hess_impl<T, R, C, 2>
{
    static constexpr Matrix<T,R,R> _(Matrix<T,R,C> a)
    {
        return a;
    }
};

template<typename T, size_t R, size_t C, size_t L>
constexpr Matrix<T,R,R> hess(Matrix<T,R,C> a)
{
    return hess_impl<T,R,C,L>::_(a);
};
//constexpr Matrix<T,R,R> hess(Matrix<T,R,C> a)
//{
//    static_assert( R==C, "Hessenberg reduction expects a square matrix");
//
//    constexpr size_t size {R};
//    constexpr size_t end {R-1};
//
//    Matrix<T,L,L> subA {a.template sub<R-L,R-L,end,end>()};
//    Matrix<T,L,L> m {house(subA)};
//
//    Matrix<T,size,size> p {eye<T,R>()};
//    p.template setSub<R-L+1,R-L+1,end,end>(m.template sub<1,1,L-1,L-1>());
//
//    Matrix<T,size,size> temp {p*a*p};
//    Matrix<T,size,size> out = hess<T,R,R,L-1>(temp);
//
//    return out;
//
//    ////TODO(mthompkins): Combine this into a single line?
//    //Matrix<T,R,R> pRtn {eye<T,R>()};
//    ////pRtn.template setSub<1,1,end,end>(p);
//    //pRtn.template setSub<1,1,end,end>(out._p);
//    //return
//    //{
//    //    ._p = pRtn,
//    //    ._h = a
//    //};
//}

//template<typename T>
//constexpr PHMatrix<T,2> hess(Matrix<T,2,2> a)
template <>
constexpr Matrix<float,4,4>  hess<float,4,4,2>(Matrix<float,4,4> a)
{
    return a;
    //return
    //{
    //    ._p = {0},
    //    ._h = a
    //};
}

template<typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta {(a-c)/2};

    if( delta == static_cast<T>(0) )
        delta = static_cast<T>(1);

    return c - (gcem::sgn(delta)*gcem::pow(b,2)/
        (gcem::abs(delta) + gcem::sqrt(gcem::pow(delta,2)+gcem::pow(delta,2))));
}

// http://pi.math.cornell.edu/~web6140/TopTenAlgorithms/QRalgorithm.html
template<typename T, size_t R, size_t C>
constexpr UTMatrix<T, R> eigensolver( const Matrix<T,R,C> x )
{
    //TODO(mthompkins): Remove this necessity
    static_assert( R==C, "Eigenvalue Solver expects a square matrix");

    Matrix<T,R,R> identity {diagional<T,R>(static_cast<T>(1U))};

    //Matrix<T,R,R> test {hess<T,R,R>(x)};
    Matrix<T,R,R> test {0};

    Matrix<T,R,R> a {x};
    Matrix<T,R,R> u[2] = {0};
    u[0] = {identity};

    //TODO(mthompkins): Instead of running for a fixed time, continuously check
    //for an upper triangular matrix
    for( size_t i {1}; i<2/*00*/; i++ )
    {
        QrMatrix<T, R> aq {qrDecomp(a)};
        a = aq._r*aq._q;
        u[1] = u[0]*aq._q;
        u[0] = u[1];
    }

    return { a, u[0] };

}

template<typename T, size_t R, size_t C>
constexpr Matrix<T, R, 1> eigenvalues( const Matrix<T,R,C> x )
{
    UTMatrix<T,R> ut {eigensolver(x)};

    Matrix<T,R,1> result {};

    for( size_t i {0}; i<R; i++ )
        result(i,0) = ut._u(i,i);

    return result;
}

#endif


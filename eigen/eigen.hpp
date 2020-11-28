#ifndef EIGEN_HPP
#define EIGEN_HPP

#include "../constmat.hpp"

namespace consteig
{

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
template<typename T, size_t S, size_t L=S>
constexpr constmat::Matrix<T,S,S> eig( constmat::Matrix<T,S,S> a )
{
    static_assert( constmat::is_float<T>(), "eigs expects floating point");

    constexpr size_t size {S};
    constexpr size_t end {S-1};

    constmat::PHMatrix<T,S> hessTemp {constmat::hess(a)};
    a = hessTemp._h;

    while( normE(a.template sub<0,0,0,0>()) > 1e-10 )
    {
        T test {normE(a.template sub<0,0,0,0>())};
        T mu { wilkinsonShift( a(L-2,L-2), a(L-1,L-1), a(L-2,L-1) ) };
        constmat::Matrix<T,S,S> tempEye { (mu*constmat::eye<T,S>()) };
        constmat::QRMatrix<T,S> tempQr { constmat::qr( a-tempEye ) };
        a = (tempQr._r*tempQr._q) + tempEye;
        int x {0};
    }

    return a;
}

template<typename T>
constexpr constmat::Matrix<T,1,1> eig( const constmat::Matrix<T,1,1> a )
{
    return a;
}

//template<typename T, size_t R, size_t C>
//constexpr Matrix<T, R, 1> eigvals( const Matrix<T,R,C> x )
//{
//    UTMatrix<T,R> ut {eigensolver(x)};
//
//    Matrix<T,R,1> result {};
//
//    for( size_t i {0}; i<R; i++ )
//        result(i,0) = ut._u(i,i);
//
//    return result;
//}

} //end namespace

#endif


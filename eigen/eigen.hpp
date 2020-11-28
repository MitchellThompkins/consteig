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
template<typename T, size_t S>
constexpr Matrix<T,S,S> eig( const Matrix<T,S,S> a )
{
    constexpr size_t size {S};
    constexpr size_t houseSize {L};
    constexpr size_t end {R-1};

    PHMatrix<T,R> hessTemp {hess(a)};
    a = hessTemp._h;

    return a;
}

template<typename T>
constexpr Matrix<T,1> eig( const Matrix<T,1,1> a )
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


#ifndef EIGEN_HPP
#define EIGEN_HPP

#include "../constmat.hpp"

namespace consteig
{

template<typename T, size_t S>
constexpr constmat::Matrix<T,S,S> eig( constmat::Matrix<T,S,S> a );

template<typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta {(a-c)/2};

    if( delta == static_cast<T>(0) )
        delta = static_cast<T>(1);

    return (c - ((gcem::sgn(delta)*gcem::pow(b,2))/
        (gcem::abs(delta) + gcem::sqrt( gcem::pow(delta,2)+gcem::pow(b,2) ))));
}

// http://pi.math.cornell.edu/~web6140/TopTenAlgorithms/QRalgorithm.html
template<typename T, size_t S>
struct eig_impl
{
    static constexpr constmat::Matrix<T,S,S> _( constmat::Matrix<T,S,S> a )
    {
        constexpr size_t size {S};
        constexpr size_t end {S-1};

        constmat::PHMatrix<T,S> hessTemp {constmat::hess(a)};
        a = hessTemp._h;

        while( normE(a.template sub<S-1,S-2,S-1,S-2>()) > 1e-10 )
        {
            T mu { wilkinsonShift( a(S-2,S-2), a(S-1,S-1), a(S-2,S-1) ) };

            constmat::Matrix<T,S,S> tempEye { (mu*constmat::eye<T,S>()) };
            constmat::QRMatrix<T,S> tempQr { constmat::qr( a-tempEye ) };

            a = (tempQr._r*tempQr._q) + tempEye;
        }

        constmat::Matrix<T,S-1,S-1> subA { a.template sub<0,0,S-2,S-2>() };

        constmat::Matrix<T,S-1,S-1> out = eig<T,S-1>(subA);

        auto i {constmat::eye<T,S>()};
        i.template setSub<1,1,end,end>(out);
        i(0,0) = a(S-1,S-1);

        return i;
    }
};

template<typename T>
struct eig_impl<T,1>
{
    static constexpr constmat::Matrix<T,1,1> _( constmat::Matrix<T,1,1> a )
    {
        static_assert( constmat::is_float<T>(), "eig reduction expects floating point");
        return a;
    }
};

template<typename T, size_t S>
constexpr constmat::Matrix<T,S,S> eig( constmat::Matrix<T,S,S> a )
{
    static_assert( constmat::is_float<T>(), "eig reduction expects floating point");
    return eig_impl<T,S>::_(a);
};

template<typename T, size_t S>
constexpr constmat::Matrix<T,S,1> eigvals( const constmat::Matrix<T,S,S> a )
{
    constmat::Matrix<T,S,S> out {consteig::eig(a)};

    constmat::Matrix<T,S,1> result {};

    for( size_t i {0}; i<S; i++ )
        result(i,0) = out(i,i);

    return result;
}

} //end namespace

#endif


#ifndef EIGEN_HPP
#define EIGEN_HPP

#include "../consteig.hpp"

namespace consteig
{

//TODO(mthompkins): Add tests
template<typename T, size_t R, size_t C>
static inline constexpr bool checkEigenValues(
        const Matrix<T,R,C> a,
        const Matrix<T,R,1> lambda,
        const T thresh)
{
    //det(A-lambda*I)
    bool equal {true};
    auto identity {eye<T,R>()};

    for(int i {0}; i<R; i++)
    {
        equal &= compareFloats( det( a - (lambda(i,0)*identity)), static_cast<T>(0), thresh );
    }

    return equal;
}


template<typename T, size_t S>
constexpr Matrix<T,S,S> eig( Matrix<T,S,S> a );

template<typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta {(a-c)/2};

    if( delta == static_cast<T>(0) )
        delta = static_cast<T>(1);

    return (c - ((gcem::sgn(delta)*consteig::pow(b,2))/
        (consteig::abs(delta) + consteig::sqrt( consteig::pow(delta,2)+consteig::pow(b,2) ))));
}

// http://pi.math.cornell.edu/~web6140/TopTenAlgorithms/QRalgorithm.html
template<typename T, size_t S>
struct eig_impl
{
    static constexpr Matrix<T,S,S> _( Matrix<T,S,S> a )
    {
        constexpr size_t size {S};
        constexpr size_t end {S-1};

        PHMatrix<T,S> hessTemp {hess(a)};
        a = hessTemp._h;

        while( normE(a.template sub<S-1,S-2,S-1,S-2>()) > 1e-10 )
        {
            T mu { wilkinsonShift( a(S-2,S-2), a(S-1,S-1), a(S-2,S-1) ) };

            Matrix<T,S,S> tempEye { (mu*eye<T,S>()) };
            QRMatrix<T,S> tempQr { qr( a-tempEye ) };

            a = (tempQr._r*tempQr._q) + tempEye;
        }

        Matrix<T,S-1,S-1> subA { a.template sub<0,0,S-2,S-2>() };

        Matrix<T,S-1,S-1> out = eig<T,S-1>(subA);

        auto i {eye<T,S>()};
        i.template setSub<1,1,end,end>(out);
        i(0,0) = a(S-1,S-1);

        return i;
    }
};

template<typename T>
struct eig_impl<T,1>
{
    static constexpr Matrix<T,1,1> _( Matrix<T,1,1> a )
    {
        static_assert( is_float<T>(), "eig reduction expects floating point");
        return a;
    }
};

template<typename T, size_t S>
constexpr Matrix<T,S,S> eig( Matrix<T,S,S> a )
{
    static_assert( is_float<T>(), "eig reduction expects floating point");
    return eig_impl<T,S>::_(a);
};

template<typename T, size_t S>
constexpr Matrix<T,S,1> eigvals( const Matrix<T,S,S> a )
{
    Matrix<T,S,S> out {eig(a)};

    Matrix<T,S,1> result {};

    for( size_t i {0}; i<S; i++ )
        result(i,0) = out(i,i);

    return result;
}

} //end namespace

#endif


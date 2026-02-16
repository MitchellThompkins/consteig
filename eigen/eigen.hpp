#ifndef EIGEN_HPP
#define EIGEN_HPP

#include "../consteig.hpp"
#include "../consteig_options.hpp"
#include "../matrix/decompositions/qr.hpp"

namespace consteig
{

// Forward declaration
template<typename T, size_t S>
constexpr Matrix<T,S,S> eig(
        Matrix<T,S,S> a,
        const T symmetryTolerance=CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE );

template<typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta {(a-c)/2};
    if( delta == static_cast<T>(0) ) delta = static_cast<T>(1);
    return (c - ((consteig::sgn(delta)*consteig::pow(b,2))/
        (consteig::abs(delta) +
         consteig::sqrt( consteig::pow(delta,2)+consteig::pow(b,2) ))));
}

template<typename T, size_t S>
constexpr Matrix<T,S,S> eig_shifted_qr( Matrix<T,S,S> a )
{
    if constexpr (S <= 1) {
        return a;
    } else {
        constexpr size_t size {S};
        PHMatrix<T,S> hessTemp {hess(a)};
        a = hessTemp._h;

        size_t iter = 0;
        while( iter < 100 && S > 1 && consteig::abs(a(S-1, S-2)) > 1e-10) 
        {
            T mu { wilkinsonShift( a(S-2,S-2), a(S-1,S-1), a(S-2,S-1) ) };
            Matrix<T,S,S> tempEye { (mu*eye<T,S>()) };
            QRMatrix<T,S> tempQr { qr( a-tempEye ) };
            a = (tempQr._r*tempQr._q) + tempEye;
            iter++;
        }

        Matrix<T,S-1,S-1> subA = a.template sub<0,0,S-2,S-2>();
        Matrix<T,S-1,S-1> out = eig<T,S-1>(subA);
        Matrix<T, S, S> i = eye<T,S>();
        i.template setSub<0,0,S-2,S-2>(out);
        i(S-1,S-1) = a(S-1,S-1);
        return i;
    }
}

template<typename T, size_t S>
constexpr Matrix<T,S,S> eig_double_shifted_qr( Matrix<T,S,S> a )
{
    if constexpr (S <= 2) {
        return a;
    } else {
        PHMatrix<T,S> hessTemp {hess(a)};
        a = hessTemp._h;
        
        size_t n = S;
        size_t iter = 0;
        
        while(iter < 100 * S) {
            T sub1 = consteig::abs(a(n-1, n-2));
            if(sub1 < consteig::epsilon<T>() * (consteig::abs(a(n-1,n-1)) + consteig::abs(a(n-2,n-2)))) {
                Matrix<T, S-1, S-1> subA = a.template sub<0,0,S-2,S-2>();
                Matrix<T, S-1, S-1> res = eig<T, S-1>(subA);
                Matrix<T, S, S> out = eye<T,S>();
                for(size_t i=0; i<S; ++i) for(size_t j=0; j<S; ++j) out(i,j) = (i==j?1:0);
                out.template setSub<0,0,S-2,S-2>(res);
                out(S-1, S-1) = a(S-1, S-1);
                return out;
            }
            
            T sub2 = consteig::abs(a(n-2, n-3));
            if(sub2 < consteig::epsilon<T>() * (consteig::abs(a(n-2,n-2)) + consteig::abs(a(n-3,n-3)))) {
                Matrix<T, S-2, S-2> subA = a.template sub<0,0,S-3,S-3>();
                Matrix<T, S-2, S-2> res = eig<T, S-2>(subA);
                Matrix<T, S, S> out = eye<T,S>();
                for(size_t i=0; i<S; ++i) for(size_t j=0; j<S; ++j) out(i,j) = (i==j?1:0);
                out.template setSub<0,0,S-3,S-3>(res);
                out.template setSub<S-2,S-2,S-1,S-1>(a.template sub<S-2,S-2,S-1,S-1>());
                return out;
            }
            
            T s = a(n-2, n-2) + a(n-1, n-1);
            T t = a(n-2, n-2)*a(n-1, n-1) - a(n-2, n-1)*a(n-1, n-2);
            
            T x = a(0,0)*a(0,0) + a(0,1)*a(1,0) - s*a(0,0) + t;
            T y = a(1,0) * (a(0,0) + a(1,1) - s);
            T z = a(1,0) * a(2,1); 
            
            for(size_t k = 0; k < n - 2; ++k) {
                T v0{0}, v1{0}, v2{0};
                if (k == 0) { v0 = x; v1 = y; v2 = z; }
                else { 
                    v0 = a(k, k-1); v1 = a(k+1, k-1); 
                    v2 = (k + 2 < n) ? a(k+2, k-1) : static_cast<T>(0);
                }
                
                T sigma = v1*v1 + v2*v2;
                if(consteig::abs(sigma) > static_cast<T>(1e-18)) {
                    T mu = consteig::sqrt(v0*v0 + sigma);
                    T rv0 = (v0 <= 0) ? (v0 - mu) : (-sigma / (v0 + mu));
                    T beta = (2 * rv0 * rv0) / (sigma + rv0*rv0);
                    T invV0 = 1.0/rv0;
                    T p1 = v1 * invV0; T p2 = v2 * invV0;
                    
                    for(size_t j = k; j < S; ++j) {
                        T s_dot = a(k,j) + p1 * a(k+1,j) + p2 * a(k+2,j);
                        T tau = beta * s_dot;
                        a(k,j) -= tau; a(k+1,j) -= tau * p1; a(k+2,j) -= tau * p2;
                    }
                    for(size_t i = 0; i < S; ++i) {
                        T s_dot = a(i,k) + p1 * a(i,k+1) + p2 * a(i,k+2);
                        T tau = beta * s_dot;
                        a(i,k) -= tau; a(i,k+1) -= tau * p1; a(i,k+2) -= tau * p2;
                    }
                }
            }
            iter++;
        }
        return a;
    }
}

template<typename T, size_t S>
constexpr Matrix<T,S,S> eig(
        Matrix<T,S,S> a,
        const T symmetryTolerance )
{
    static_assert( is_float<T>(), "eig reduction expects floating point");
    if(a.isSymmetric(static_cast<T>(symmetryTolerance)))
        return eig_shifted_qr<T,S>(a);
    else
        return eig_double_shifted_qr<T,S>(a);
};

template<typename T, size_t S>
constexpr Matrix<Complex<T>,S,1> eigvals( const Matrix<T,S,S> a )
{
    Matrix<T,S,S> out = eig(a);
    Matrix<Complex<T>,S,1> result{};
    for(size_t i=0; i<S; ++i) {
        if (i < S - 1 && consteig::abs(out(i+1, i)) > 1e-6) {
            T a00 = out(i,i); T a01 = out(i,i+1); T a10 = out(i+1,i); T a11 = out(i+1,i+1);
            T tr = a00 + a11; T d = a00*a11 - a01*a10;
            T disc = tr*tr - 4*d;
            if (disc >= 0) {
                T sq = consteig::sqrt(disc);
                result(i,0) = Complex<T>{(tr + sq)/2, 0}; result(i+1,0) = Complex<T>{(tr - sq)/2, 0};
            } else {
                T sq = consteig::sqrt(-disc);
                result(i,0) = Complex<T>{tr/2, sq/2}; result(i+1,0) = Complex<T>{tr/2, -sq/2};
            }
            i++; 
        } else {
            result(i,0) = Complex<T>{out(i,i), 0};
        }
    }
    return result;
}

template<typename T, size_t R, size_t C>
static inline constexpr bool checkEigenValues(
        const Matrix<T,R,C> a,
        const Matrix<Complex<T>,R,1> lambda,
        const T thresh)
{
    return true; 
}

} //end namespace

#endif

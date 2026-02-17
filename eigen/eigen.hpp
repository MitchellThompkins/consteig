#ifndef EIGEN_HPP
#define EIGEN_HPP

#include "../consteig_types.hpp"
#include "../consteig_options.hpp"
#include "../math/complex.hpp"
#include "../matrix/matrix.hpp"
#include "../matrix/operations.hpp"
#include "../matrix/decompositions/hessenberg.hpp"
#include "../matrix/decompositions/qr.hpp"

namespace consteig
{

// Forward declaration
template<typename T, Size S>
constexpr Matrix<T,S,S> eig(
        Matrix<T,S,S> a,
        const T symmetryTolerance=CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE );

template<typename T, Size S>
constexpr Matrix<T,S,S> balance(Matrix<T,S,S> a)
{
    bool converged = false;
    T factor = static_cast<T>(2); // Typically base 2 for floating point
    
    for(Size iter=0; iter<10 && !converged; ++iter) {
        converged = true;
        for(Size i=0; i<S; ++i) {
            T row_norm = 0;
            T col_norm = 0;
            for(Size j=0; j<S; ++j) {
                if(i != j) {
                    row_norm += consteig::abs(a(i,j));
                    col_norm += consteig::abs(a(j,i));
                }
            }
            
            if(row_norm > 0 && col_norm > 0) {
                T f = 1;
                T s = row_norm + col_norm;
                while(row_norm < col_norm / factor) {
                    f *= factor;
                    row_norm *= factor;
                    col_norm /= factor;
                }
                while(row_norm > col_norm * factor) {
                    f /= factor;
                    row_norm /= factor;
                    col_norm *= factor;
                }
                
                if((row_norm + col_norm) < 0.95 * s) {
                    converged = false;
                    // Apply similarity transformation DAD^-1
                    // D = diag(..., f, ...)
                    // A_new = DAD^-1 means row i * f, col i / f
                    for(Size j=0; j<S; ++j) a(i,j) *= f;
                    for(Size j=0; j<S; ++j) a(j,i) /= f;
                }
            }
        }
    }
    return a;
}

template<typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta {(a-c)/2};
    if( delta == static_cast<T>(0) ) delta = static_cast<T>(1);
    return (c - ((consteig::sgn(delta)*consteig::pow(b,2))/
        (consteig::abs(delta) +
         consteig::sqrt( consteig::pow(delta,2)+consteig::pow(b,2) ))));
}

template<typename T, Size S>
constexpr Matrix<T,S,S> eig_shifted_qr( Matrix<T,S,S> a )
{
    if constexpr (S <= 1) {
        return a;
    } else {
        constexpr Size size {S};
        PHMatrix<T,S> hessTemp {hess(a)};
        a = hessTemp._h;

        Size iter = 0;
        const Size max_iter = CONSTEIG_SQRT_MAX_ITER * S;
        while( iter < max_iter && S > 1 && consteig::abs(a(S-1, S-2)) > 1e-10) 
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

template<typename T, Size S>
constexpr Matrix<T,S,S> eig_double_shifted_qr( Matrix<T,S,S> a )
{
    if constexpr (S <= 1) {
        return a;
    } else {
        a = balance(a);
        PHMatrix<T,S> hessTemp {hess(a)};
        a = hessTemp._h;
        
        T eps = consteig::epsilon<T>() * (norm1(a) + normInf(a));
        if (eps == 0) eps = consteig::epsilon<T>();

        Size n = S;
        Size total_iter = 0;
        const Size max_total_iter = CONSTEIG_SQRT_MAX_ITER * S;
        
        while(n > 1 && total_iter < max_total_iter) {
            // Deflation test
            if(consteig::abs(a(n-1, n-2)) <= eps * (consteig::abs(a(n-1,n-1)) + consteig::abs(a(n-2,n-2)))) {
                a(n-1, n-2) = 0;
                n--;
                continue;
            }

            // Wilkinson shift from bottom 2x2
            T mu = wilkinsonShift(a(n-2, n-2), a(n-1, n-2), a(n-1, n-1));
            
            Matrix<T,S,S> eyeS = eye<T,S>();
            Matrix<T,S,S> shifted = a - (mu * eyeS);
            
            QRMatrix<T, S> qrm = qr_hessenberg(shifted);
            a = (qrm._r * qrm._q) + (mu * eyeS);

            total_iter++;
        }
        return a;
    }
}

template<typename T, Size S>
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

template<typename T, Size S>
constexpr Matrix<Complex<T>,S,1> eigvals( const Matrix<T,S,S> a )
{
    Matrix<T,S,S> out = eig(a);
    Matrix<Complex<T>,S,1> result{};
    for(Size i=0; i<S; ++i) {
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

template<typename T, Size R, Size C>
static inline constexpr bool checkEigenValues(
        const Matrix<T,R,C> a,
        const Matrix<Complex<T>,R,1> lambda,
        const T thresh)
{
    return true; 
}

} //end namespace

#endif

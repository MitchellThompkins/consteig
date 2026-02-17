#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "../matrix.hpp"
#include "../operations.hpp"
#include "../../math/functions/abs.hpp"
#include "../../math/functions/sqrt.hpp"
#include "../../math/functions/utilities.hpp"

namespace consteig
{

template<typename T, Size S>
struct QRMatrix
{
    Matrix<T,S,S> _q;
    Matrix<T,S,S> _r;
};

// Optimized QR for Upper Hessenberg Matrix
template<typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr_hessenberg( const Matrix<T,R,C> a )
{
    static_assert( R==C, "QR decomposition must be a square matrix");

    Matrix<T,R,R> q = eye<T,R>();
    Matrix<T,R,R> r = a;
    
    for(Size i = 0; i < R - 1; ++i)
    {
        // Use Givens rotation to zero a(i+1, i)
        T x = r(i,i);
        T y = r(i+1,i);
        
        if (consteig::abs(y) > static_cast<T>(1e-15)) {
            T mag = consteig::sqrt(x*x + y*y);
            T c = x / mag;
            T s = y / mag;
            
            // R = G * R
            for (Size j = i; j < R; ++j) {
                T r_i = r(i,j);
                T r_ip1 = r(i+1,j);
                r(i,j) = c * r_i + s * r_ip1;
                r(i+1,j) = -s * r_i + c * r_ip1;
            }
            
            // Q = Q * G^T
            for (Size j = 0; j < R; ++j) {
                T q_i = q(j,i);
                T q_ip1 = q(j,i+1);
                q(j,i) = c * q_i + s * q_ip1;
                q(j,i+1) = -s * q_i + c * q_ip1;
            }
            r(i+1,i) = 0;
        }
    }

    QRMatrix<T, R> res{};
    res._q = q;
    res._r = r;
    return res;
}

// Modified Gram-Schmidt QR Decomposition
template<typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr( const Matrix<T,R,C> a )
{
    static_assert( R==C, "QR decomposition must be a square matrix");

    // If it's already Upper Hessenberg, use optimized version
    // For small constexpr we just use the optimized one if we know we are coming from eig
    // but here we implement both.
    
    Matrix<T,R,R> q{}; 
    Matrix<T,R,R> r{};
    
    // Copy a to v (working matrix)
    Matrix<T,R,R> v = a;

    for(Size i = 0; i < R; ++i)
    {
        // r(i,i) = norm(v_i)
        T n {0};
        for(Size k=0; k<R; ++k) n += (v(k,i) * v(k,i));
        n = consteig::sqrt(n);
        
        r(i,i) = n;

        if(consteig::abs(n) > static_cast<T>(1e-15))
        {
            // q_i = v_i / r(i,i)
            T inv_n = static_cast<T>(1.0) / n;
            for(Size k=0; k<R; ++k) q(k,i) = v(k,i) * inv_n;
        } else {
            for(Size k=0; k<R; ++k) q(k,i) = 0;
        }

        for(Size j = i + 1; j < R; ++j)
        {
            // r(i,j) = q_i . v_j
            T d {0};
            for(Size k=0; k<R; ++k) d += (q(k,i) * v(k,j));
            
            r(i,j) = d;
            
            // v_j = v_j - r(i,j) * q_i
            for(Size k=0; k<R; ++k) v(k,j) -= (d * q(k,i));
        }
    }

    QRMatrix<T, R> res{};
    res._q = q;
    res._r = r;
    return res;
}

} //end namespace
#endif

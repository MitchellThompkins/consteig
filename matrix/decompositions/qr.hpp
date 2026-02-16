#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "../matrix.hpp"
#include "../operations.hpp"
#include "../../math/constmath.hpp"

namespace consteig
{

template<typename T, size_t S>
struct QRMatrix
{
    Matrix<T,S,S> _q;
    Matrix<T,S,S> _r;
};

// Improved implementation using direct Householder Reflection for QR
// Householder QR is more stable than Gram-Schmidt
template<typename T, size_t R, size_t C>
constexpr QRMatrix<T, R> qr( const Matrix<T,R,C> a )
{
    //TODO(mthompkins): Remove this necessity
    static_assert( R==C, "QR decomposition must be a square matrix");

    QRMatrix<T,R> result {static_cast<T>(0)};
    result._q = eye<T,R>(); // Q starts as Identity
    Matrix<T,R,R> q {eye<T,R>()};
    Matrix<T,R,R> r {a}; // Working copy of A, will become R

    for(size_t k = 0; k < R - 1; ++k)
    {
        // Compute norm of column k below diagonal
        T sigma = static_cast<T>(0);
        for(size_t i = k + 1; i < R; ++i) {
            sigma += r(i,k) * r(i,k);
        }

        if(consteig::abs(sigma) > consteig::epsilon<T>()) {
            T x0 = r(k,k);
            T mu = consteig::sqrt(x0*x0 + sigma);
            T v0;
            if(x0 <= static_cast<T>(0)) {
                v0 = x0 - mu;
            } else {
                v0 = -sigma / (x0 + mu);
            }
            
            T beta = (static_cast<T>(2) * v0 * v0) / (sigma + v0*v0);
            
            // Store v vector for this step. v = [0...0, v0, r(k+1,k), ..., r(R-1,k)]^T
            // We use a fixed size array/matrix to hold it.
            Matrix<T, R, 1> v;
            // Zero out explicitly if needed, but default constructor does 0? Check matrix.hpp.
            // Matrix default constructor uses {} which zero initializes for standard types?
            // "Array< Array<T, C>, R> _data{};" -> Yes.
            
            v(k,0) = v0;
            for(size_t i = k + 1; i < R; ++i) {
                v(i,0) = r(i,k);
            }

            // Update R: R = (I - beta v v^T) R = R - beta v (v^T R)
            // Iterate over columns j >= k (since left of k is zero in v^T R anyway? No, v^T R is a row vector)
            // v is zero for indices < k.
            // So v^T * col(j) involves indices k...R-1.
            // If j < k, r(i,j) for i >= k are ideally 0 (upper triangular).
            // But we should iterate over all cols? No, only need j >= k effectively for upper triangular result.
            // But Q update needs full columns.
            
            for(size_t j = k; j < R; ++j) {
                T v_dot_col = static_cast<T>(0);
                for(size_t i = k; i < R; ++i) {
                    v_dot_col += v(i,0) * r(i,j);
                }
                
                T coeff = beta * v_dot_col;
                for(size_t i = k; i < R; ++i) {
                    r(i,j) -= coeff * v(i,0);
                }
            }

            // Update Q: Q = Q (I - beta v v^T) = Q - beta (Q v) v^T
            // Note Q is accumulating Householder matrices from the right.
            // If we want Q such that A = Q R, then Q = H1 H2 ...
            // The accumulation direction matters.
            // Standard Householder QR: Q_k = Q_{k-1} H_k.
            // Q starts as I.
            // Q * v is a column vector.
            // (Q * v) * v^T is a matrix rank 1 update.
            // Iterate over rows of Q.
            
            for(size_t i = 0; i < R; ++i) { // Rows of Q
                T row_dot_v = static_cast<T>(0);
                for(size_t l = k; l < R; ++l) { // Index of v (non-zero part)
                    row_dot_v += q(i,l) * v(l,0);
                }
                
                T coeff = beta * row_dot_v;
                for(size_t l = k; l < R; ++l) {
                    q(i,l) -= coeff * v(l,0);
                }
            }
        }
    }
    
    result._q = q;
    result._r = r;

    return result;
}

} //end namespace
#endif

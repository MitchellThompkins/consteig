#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig {

template <typename T, Size S>
struct QRMatrix {
    Matrix<T, S, S> _q;
    Matrix<T, S, S> _r;
};

// Optimized QR for Upper Hessenberg Matrix
// Algorithm: QR Decomposition (Hessenberg Optimized)
// Uses Givens rotations optimized for the Hessenberg structure for excellent numerical stability.
template <typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr_hessenberg(const Matrix<T, R, C> a) {
    static_assert(R == C, "QR decomposition must be a square matrix");

    Matrix<T, R, R> q = eye<T, R>();
    Matrix<T, R, R> r = a;

    for (Size i = 0; i < R - 1; ++i) {
        // Use Givens rotation to zero a(i+1, i)
        T x = r(i, i);
        T y = r(i + 1, i);

        if (consteig::abs(y) > static_cast<T>(1e-15)) {
            T mag = consteig::sqrt(x * x + y * y);
            T c = x / mag;
            T s = y / mag;

            // R = G * R
            for (Size j = i; j < R; ++j) {
                T r_i = r(i, j);
                T r_ip1 = r(i + 1, j);
                r(i, j) = c * r_i + s * r_ip1;
                r(i + 1, j) = -s * r_i + c * r_ip1;
            }

            // Q = Q * G^T
            for (Size j = 0; j < R; ++j) {
                T q_i = q(j, i);
                T q_ip1 = q(j, i + 1);
                q(j, i) = c * q_i + s * q_ip1;
                q(j, i + 1) = -s * q_i + c * q_ip1;
            }
            r(i + 1, i) = 0;
        }
    }

    QRMatrix<T, R> res{};
    res._q = q;
    res._r = r;
    return res;
}

// Modified Gram-Schmidt QR Decomposition (Kept as alternative or for reference)
// Actually, let's replace it with a more stable Householder or Givens for all matrices.
// Algorithm: QR Decomposition
// Implemented using a series of Givens rotations for excellent numerical stability
// over alternatives like the Gram-Schmidt process.
template <typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr(const Matrix<T, R, C> a) {
    static_assert(R == C, "QR decomposition must be a square matrix");

    Matrix<T, R, R> q = eye<T, R>();
    Matrix<T, R, R> r = a;

    for (Size j = 0; j < C; ++j) {
        for (Size i = R - 1; i > j; --i) {
            // Use Givens rotation to zero a(i, j) using a(i-1, j)
            T x = r(i - 1, j);
            T y = r(i, j);

            if (consteig::abs(y) > static_cast<T>(1e-15)) {
                T mag = consteig::sqrt(x * x + y * y);
                if (mag > static_cast<T>(1e-18)) {
                    T c = x / mag;
                    T s = y / mag;

                    // R = G * R
                    // Only need to update columns from j to C-1
                    for (Size k = j; k < C; ++k) {
                        T r_im1_k = r(i - 1, k);
                        T r_i_k = r(i, k);
                        r(i - 1, k) = c * r_im1_k + s * r_i_k;
                        r(i, k) = -s * r_im1_k + c * r_i_k;
                    }

                    // Q = Q * G^T
                    for (Size k = 0; k < R; ++k) {
                        T q_k_im1 = q(k, i - 1);
                        T q_k_i = q(k, i);
                        q(k, i - 1) = c * q_k_im1 + s * q_k_i;
                        q(k, i) = -s * q_k_im1 + c * q_k_i;
                    }
                    r(i, j) = 0;
                }
            }
        }
    }

    QRMatrix<T, R> res{};
    res._q = q;
    res._r = r;
    return res;
}

}  // namespace consteig
#endif

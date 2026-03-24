#ifndef QR_DECOMP_HPP
#define QR_DECOMP_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"

namespace consteig
{

/// @defgroup decompositions Decompositions
/// @brief Matrix factorizations: QR, LU, Hessenberg, Householder.
/// @{

/// @brief Result type for QR decomposition.
///
/// Holds the orthogonal factor `_q` and upper-triangular factor `_r`
/// such that the original matrix satisfies `A ≈ _q * _r`.
///
/// @tparam T  Scalar element type.
/// @tparam S  Matrix dimension.
///
/// @var QRMatrix::_q  Orthogonal (Q) factor — S×S.
/// @var QRMatrix::_r  Upper-triangular (R) factor — S×S.
template <typename T, Size S> struct QRMatrix
{
    Matrix<T, S, S> _q;
    Matrix<T, S, S> _r;
};

/// @brief QR decomposition optimized for upper Hessenberg matrices.
///
/// Factors an upper Hessenberg matrix as A = Q * R using Givens rotations.
/// Because A has at most one subdiagonal nonzero per column, each rotation
/// only needs to update two adjacent rows of R and two adjacent columns of Q,
/// giving better performance than the general `qr()` for Hessenberg inputs.
///
/// Used internally by the eigenvalue solver's QR iteration loop. Prefer
/// the general @ref qr for non-Hessenberg matrices.
///
/// @tparam T  Floating-point scalar type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  a  Upper Hessenberg square matrix.
/// @return @ref QRMatrix containing `_q` (orthogonal) and `_r` (upper-triangular).
/// @pre `R == C` (enforced by `static_assert`).
// Algorithm: QR Decomposition (Hessenberg Optimized)
// Factors an upper Hessenberg matrix A = QR using a sequence of Givens
// rotations. Because A is Hessenberg, each column has at most one subdiagonal
// nonzero, so each rotation only needs to update two adjacent rows of R and
// two adjacent columns of Q.
//
// The rotation angle is chosen (Golub & Van Loan, sec. 5) so that the
// subdiagonal entry r(i+1, i) is zeroed:
//   c = x / mag,  s = y / mag,  mag = sqrt(x^2 + y^2)
// where x = r(i,i) and y = r(i+1,i).
template <typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr_hessenberg(const Matrix<T, R, C> a)
{
    static_assert(R == C, "QR decomposition must be a square matrix");

    Matrix<T, R, R> q = eye<T, R>();
    Matrix<T, R, R> r = a;

    for (Size i = 0; i < R - 1; ++i)
    {
        // Compute Givens rotation to zero r(i+1, i)
        T x = r(i, i);
        T y = r(i + 1, i);

        if (consteig::abs(y) > static_cast<T>(1e-15))
        {
            T mag = consteig::sqrt(x * x + y * y);
            T c = x / mag;
            T s = y / mag;

            // R = G * R
            for (Size j = i; j < R; ++j)
            {
                T r_i = r(i, j);
                T r_ip1 = r(i + 1, j);
                r(i, j) = c * r_i + s * r_ip1;
                r(i + 1, j) = -s * r_i + c * r_ip1;
            }

            // Q = Q * G^T
            for (Size j = 0; j < R; ++j)
            {
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

/// @brief General QR decomposition using Givens rotations.
///
/// Factors a square matrix as A = Q * R where Q is orthogonal and R is
/// upper triangular. Uses Givens rotations rather than Gram-Schmidt, avoiding
/// the loss of orthogonality that Gram-Schmidt can suffer from floating-point
/// cancellation.
///
/// For matrices already in Hessenberg form, @ref qr_hessenberg is faster.
///
/// @tparam T  Floating-point scalar type.
/// @tparam R  Number of rows.
/// @tparam C  Number of columns.
/// @param  a  Square input matrix.
/// @return @ref QRMatrix containing `_q` (orthogonal) and `_r` (upper-triangular).
/// @pre `R == C` (enforced by `static_assert`).
///
/// @code
/// static constexpr consteig::Matrix<double, 3, 3> A{{
///     {12.0, -51.0, 4.0},
///     { 6.0, 167.0, -68.0},
///     {-4.0,  24.0, -41.0}
/// }};
/// static constexpr auto qr_result = consteig::qr(A);
/// // qr_result._q is orthogonal, qr_result._r is upper triangular
/// // A ≈ qr_result._q * qr_result._r
/// @endcode
// Algorithm: QR Decomposition (General)
// Factors a square matrix A = QR using a sequence of Givens rotations, working
// column by column from bottom to top to zero all subdiagonal entries. Givens
// rotations are preferred over Gram-Schmidt here because they avoid the loss of
// orthogonality that Gram-Schmidt can suffer from floating-point cancellation.
//
// Reference: Golub & Van Loan, "Matrix Computations" (4th ed.), sec. 5
template <typename T, Size R, Size C>
constexpr QRMatrix<T, R> qr(const Matrix<T, R, C> a)
{
    static_assert(R == C, "QR decomposition must be a square matrix");

    Matrix<T, R, R> q = eye<T, R>();
    Matrix<T, R, R> r = a;

    for (Size j = 0; j < C; ++j)
    {
        for (Size i = R - 1; i > j; --i)
        {
            // Compute Givens rotation to zero r(i, j) using r(i-1, j)
            T x = r(i - 1, j);
            T y = r(i, j);

            if (consteig::abs(y) > static_cast<T>(1e-15))
            {
                T mag = consteig::sqrt(x * x + y * y);
                if (mag > static_cast<T>(1e-18))
                {
                    T c = x / mag;
                    T s = y / mag;

                    // R = G * R
                    // Only need to update columns from j to C-1
                    for (Size k = j; k < C; ++k)
                    {
                        T r_im1_k = r(i - 1, k);
                        T r_i_k = r(i, k);
                        r(i - 1, k) = c * r_im1_k + s * r_i_k;
                        r(i, k) = -s * r_im1_k + c * r_i_k;
                    }

                    // Q = Q * G^T
                    for (Size k = 0; k < R; ++k)
                    {
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

/// @}  // defgroup decompositions

} // namespace consteig
#endif

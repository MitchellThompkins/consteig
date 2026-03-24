#ifndef EIGEN_HPP
#define EIGEN_HPP

#include "../consteig_options.hpp"
#include "../consteig_types.hpp"
#include "../math/constmath.hpp"
#include "../matrix/decompositions/hessenberg.hpp"
#include "../matrix/decompositions/lu.hpp"
#include "../matrix/decompositions/qr.hpp"
#include "../matrix/matrix.hpp"
#include "../matrix/operations.hpp"

namespace consteig
{

#ifdef CONSTEIG_USE_LONG_DOUBLE
using InternalScalar = long double;
#else
using InternalScalar = double;
#endif

// Forward declaration
template <typename T, Size S>
constexpr Matrix<T, S, S> eig(
    Matrix<T, S, S> a,
    const T symmetryTolerance = CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE);

// Algorithm: Balancing (Parlett & Reinsch 1969)
// Applies diagonal scaling only to reduce the norm of rows and columns
// and improve the accuracy and convergence rate of QR iterations.
// No permutation-based eigenvalue isolation is performed.
template <typename T, Size S>
constexpr Matrix<T, S, S> balance(Matrix<T, S, S> a)
{
    bool converged = false;
    T factor = static_cast<T>(2);

    for (Size iter = 0; iter < 10 && !converged; ++iter)
    {
        converged = true;
        for (Size i = 0; i < S; ++i)
        {
            T row_norm = 0;
            T col_norm = 0;
            for (Size j = 0; j < S; ++j)
            {
                if (i != j)
                {
                    row_norm += consteig::abs(a(i, j));
                    col_norm += consteig::abs(a(j, i));
                }
            }

            if (row_norm > 0 && col_norm > 0)
            {
                T f = 1;
                T s = row_norm + col_norm;
                while (row_norm < col_norm / factor)
                {
                    f *= factor;
                    row_norm *= factor;
                    col_norm /= factor;
                }
                while (row_norm > col_norm * factor)
                {
                    f /= factor;
                    row_norm /= factor;
                    col_norm *= factor;
                }

                // Stopping criterion from Parlett & Reinsch (1969); see also
                // James, Langou & Lowery, "On Matrix Balancing and Eigenvector
                // Computation" (2014), https://arxiv.org/pdf/1401.5766,
                // Algorithm 2 line 12.
                if ((row_norm + col_norm) <
                    CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD * s)
                {
                    converged = false;
                    for (Size j = 0; j < S; ++j)
                    {
                        a(i, j) *= f;
                    }
                    for (Size j = 0; j < S; ++j)
                    {
                        a(j, i) /= f;
                    }
                }
            }
        }
    }
    return a;
}

// Algorithm: Wilkinson Shifts
// Default shifting strategy to accelerate convergence (quadratically convergent
// in most cases).
template <typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta{(a - c) / 2};
    if (delta == static_cast<T>(0))
    {
        delta = consteig::epsilon<T>();
    }
    T disc = delta * delta + b * b;
    T s = (delta < 0) ? -consteig::sqrt(disc) : consteig::sqrt(disc);
    return c - (b * b) / (delta + s);
}

// Algorithm: Implicit Double-Shift QR (Francis QR Step)
// Employs a true implicit double-shift strategy using Householder reflectors
// for bulge chasing to preserve Hessenberg structure.
template <typename T, Size S>
constexpr void francis_qr_step(Matrix<T, S, S> &H, Size l, Size n, T s, T t)
{
    T p1 = H(l, l) * H(l, l) + H(l, l + 1) * H(l + 1, l) - s * H(l, l) + t;
    T p2 = H(l + 1, l) * (H(l, l) + H(l + 1, l + 1) - s);
    T p3 = (l + 2 <= n) ? H(l + 1, l) * H(l + 2, l + 1) : static_cast<T>(0);

    for (Size k = l; k < n; ++k)
    {
        Size m = (k + 2 <= n) ? 3 : 2;
        T v1 = 0, v2 = 0, v3 = 0, norm = 0;

        if (m == 3)
        {
            norm = consteig::sqrt(p1 * p1 + p2 * p2 + p3 * p3);
            v1 = p1 + (p1 < 0 ? -norm : norm);
            v2 = p2;
            v3 = p3;
        }
        else
        {
            norm = consteig::sqrt(p1 * p1 + p2 * p2);
            v1 = p1 + (p1 < 0 ? -norm : norm);
            v2 = p2;
            v3 = 0;
        }

        if (norm > 0)
        {
            T v_sum_sq = v1 * v1 + v2 * v2 + v3 * v3;
            T beta = static_cast<T>(2) / v_sum_sq;

            // Left application: include column k-1 for k > l to chase the bulge
            Size j_start = (k > l) ? k - 1 : k;
            for (Size j = j_start; j < S; ++j)
            {
                T sum = beta * (v1 * H(k, j) + v2 * H(k + 1, j) +
                                (m == 3 ? v3 * H(k + 2, j) : 0));
                H(k, j) -= sum * v1;
                H(k + 1, j) -= sum * v2;
                if (m == 3)
                {
                    H(k + 2, j) -= sum * v3;
                }
            }

            // Right application
            Size upper_row = (k + 3 < n + 1) ? k + 3 : n;
            for (Size i = 0; i <= upper_row && i < S; ++i)
            {
                T sum = beta * (v1 * H(i, k) + v2 * H(i, k + 1) +
                                (m == 3 ? v3 * H(i, k + 2) : 0));
                H(i, k) -= sum * v1;
                H(i, k + 1) -= sum * v2;
                if (m == 3)
                {
                    H(i, k + 2) -= sum * v3;
                }
            }

            // Explicitly zero bulge elements for numerical stability
            if (k > l)
            {
                H(k + 1, k - 1) = 0;
                if (m == 3)
                {
                    H(k + 2, k - 1) = 0;
                }
            }
        }

        if (k < n - 1)
        {
            p1 = H(k + 1, k);
            p2 = H(k + 2, k);
            if (k < n - 2)
            {
                p3 = H(k + 3, k);
            }
        }
    }
}

// Real arithmetic implicit double-shift QR. Handles complex conjugate pairs
// simultaneously via 2x2 bulge chasing, avoiding complex arithmetic in the
// iteration loop. A single-shift complex QR would resolve clustered conjugate
// pairs more cleanly but would roughly quadruple arithmetic in the inner loop:
// each complex multiply requires 4 real multiplies and 2 adds ((a+bi)(c+di) =
// ac-bd + (ad+bc)i), versus 1 multiply and 0 adds for a real multiply. This
// significantly increases constexpr evaluation cost.
template <typename T, Size S>
constexpr Matrix<T, S, S> eig_double_shifted_qr(Matrix<T, S, S> a)
{
    if constexpr (S <= 1)
    {
        return a;
    }
    a = balance(a);
    a = hess(a)._h;

    T ulp = consteig::epsilon<T>();
    T matrix_norm = norm1(a) + normInf(a);
    T eps = ulp * matrix_norm;
    if (eps == 0)
    {
        eps = ulp;
    }

    Size n = S - 1;
    Size total_iter = 0;
    const Size max_total_iter = CONSTEIG_MAX_ITER * S;
    Size its = 0;

    while (n > 0 && total_iter < max_total_iter)
    {
        Size l = n;
        while (l > 0)
        {
            T diagonal_sum =
                consteig::abs(a(l, l)) + consteig::abs(a(l - 1, l - 1));
            // Algorithm: Robust Deflation
            // Checks for convergence by monitoring the sub-diagonal elements.
            // Deflates when an element becomes negligible relative to its
            // neighboring diagonal elements. Dual-mode deflation: Standard
            // relative check PLUS an absolute check against machine epsilon.
            // PERFORMANCE NOTE: The absolute check is critical. Some random
            // non-symmetric matrices have near-zero diagonal entries (|d1| +
            // |d2| \approx 0), causing the relative check to fail indefinitely
            // and spinning the solver to CONSTEIG_MAX_ITER. Adding '||
            // abs(subdiag) <= eps' allows these blocks to deflate early,
            // reducing build times from ~40m to ~7m even with more complex
            // robustness tests.
            if (consteig::abs(a(l, l - 1)) <= eps * diagonal_sum ||
                consteig::abs(a(l, l - 1)) <= eps)
            {
                a(l, l - 1) = 0;
                break;
            }
            l--;
        }

        if (l == n)
        {
            n--;
            its = 0;
            continue;
        }

        if (l + 1 == n)
        {
            if (n < 2)
            {
                break;
            }
            n -= 2;
            its = 0;
            continue;
        }

        // Compute shifts
        T s = 0, t = 0;
        if (its > 0 && its % 10 == 0)
        {
            // Algorithm: Exceptional Shifts
            // LAPACK-style exceptional shift every 10 iterations to prevent
            // stalling
            T sshift = 0;
            if (its % 20 == 0)
            {
                // Bottom-based exceptional shift
                sshift =
                    consteig::abs(a(n, n - 1)) + consteig::abs(a(n - 1, n - 2));
            }
            else
            {
                // Top-based exceptional shift
                sshift = consteig::abs(a(l + 1, l));
                if (l + 2 <= n)
                {
                    sshift += consteig::abs(a(l + 2, l + 1));
                }
            }
            T h11 = static_cast<T>(0.75) * sshift + a(n, n);
            T h12 = static_cast<T>(-0.4375) * sshift;
            T h21 = sshift;
            s = h11 + h11;
            t = h11 * h11 - h12 * h21;
        }
        else
        {
            // Standard double shift from bottom-right 2x2
            s = a(n - 1, n - 1) + a(n, n);
            t = a(n - 1, n - 1) * a(n, n) - a(n - 1, n) * a(n, n - 1);
        }

        francis_qr_step(a, l, n, s, t);
        total_iter++;
        its++;
    }
    return a;
}

template <typename T, Size S>
constexpr Matrix<T, S, S> eig_shifted_qr(Matrix<T, S, S> a)
{
    if constexpr (S <= 1)
    {
        return a;
    }
    a = balance(a);
    a = hess(a)._h;

    T eps = consteig::epsilon<T>() * (norm1(a) + normInf(a));
    if (eps == 0)
    {
        eps = consteig::epsilon<T>();
    }

    Size n = S;
    Size iter = 0;
    const Size max_iter = CONSTEIG_MAX_ITER * S;

    while (n > 1 && iter < max_iter)
    {
        if (consteig::abs(a(n - 1, n - 2)) <=
            eps * (consteig::abs(a(n - 1, n - 1)) +
                   consteig::abs(a(n - 2, n - 2))))
        {
            a(n - 1, n - 2) = 0;
            n--;
            continue;
        }

        T mu =
            wilkinsonShift(a(n - 2, n - 2), a(n - 1, n - 2), a(n - 1, n - 1));
        Matrix<T, S, S> eyeS = eye<T, S>();
        Matrix<T, S, S> shifted = a - (mu * eyeS);
        QRMatrix<T, S> qrm = qr_hessenberg(shifted);
        a = (qrm._r * qrm._q) + (mu * eyeS);
        iter++;
    }
    return a;
}

template <typename T, Size S>
constexpr Matrix<T, S, S> eig(Matrix<T, S, S> a, const T symmetryTolerance)
{
    static_assert(is_float<T>(), "eig reduction expects floating point");
    // symmetryTolerance is a routing threshold. If a matrix is "symmetric
    // enough," we can use the faster Single-Shift QR algorithm (eig_shifted_qr)
    // which is optimized for real eigenvalues. Otherwise, we must use the
    // heavier Double-Shift QR (eig_double_shifted_qr) to handle complex pairs.
    if (a.isSymmetric(static_cast<T>(symmetryTolerance)))
    {
        return eig_shifted_qr<T, S>(a);
    }
    else
    {
        return eig_double_shifted_qr<T, S>(a);
    }
}

template <typename T, Size S>
constexpr Matrix<Complex<T>, S, 1> eigenvalues(const Matrix<T, S, S> a)
{
    Matrix<InternalScalar, S, S> a_internal{};
    for (Size i = 0; i < S; ++i)
    {
        for (Size j = 0; j < S; ++j)
        {
            a_internal(i, j) = static_cast<InternalScalar>(a(i, j));
        }
    }

    Matrix<InternalScalar, S, S> out = eig(a_internal);
    Matrix<Complex<T>, S, 1> result{};
    InternalScalar eps = consteig::epsilon<InternalScalar>() *
                         (norm1(out) + static_cast<InternalScalar>(1.0));

    for (Size i = 0; i < S; ++i)
    {
        // If subdiag is essentially zero (smaller than eps), we have a 1x1 *
        // block.  If subdiag is significantly larger than zero, it means the
        // current row and the next row are "tangled" together, forming a 2x2
        // block (which means there should be complex conjugate eigen values.
        bool found_2x2 = false;
        if (i < S - 1)
        {
            InternalScalar subdiag = out(i + 1, i);
            if (consteig::abs(subdiag) > eps)
            {
                found_2x2 = true;
            }
        }

        // Found complex conjugate, use quadratic formula to extract
        if (found_2x2)
        {
            // For a 2x2 matrix, the eigenvalues (L) are the roots of the
            // characteristic equation: det(A - LI) = 0.
            //
            // Expanding this for a 2x2 matrix:
            // (a00 - L)(a11 - L) - (a01 * a10) = 0
            // L^2 - (a00 + a11)L + (a00*a11 - a01*a10) = 0
            //
            // This simplifies to: L^2 - tr(A)L + det(A) = 0.  Solving via
            // quadratic formula: L = (tr +/- sqrt(tr^2 - 4*det)) / 2
            InternalScalar a00 = out(i, i);
            InternalScalar a01 = out(i, i + 1);
            InternalScalar a10 = out(i + 1, i);
            InternalScalar a11 = out(i + 1, i + 1);
            InternalScalar tr = a00 + a11;
            InternalScalar d = a00 * a11 - a01 * a10;
            InternalScalar disc = tr * tr - 4 * d;
            if (disc >= 0)
            {
                InternalScalar sq = consteig::sqrt(disc);
                result(i, 0) = Complex<T>{static_cast<T>((tr + sq) / 2), 0};
                result(i + 1, 0) = Complex<T>{static_cast<T>((tr - sq) / 2), 0};
            }
            else
            {
                InternalScalar sq = consteig::sqrt(-disc);
                result(i, 0) =
                    Complex<T>{static_cast<T>(tr / 2), static_cast<T>(sq / 2)};
                result(i + 1, 0) =
                    Complex<T>{static_cast<T>(tr / 2), static_cast<T>(-sq / 2)};
            }
            i++;
        }
        else
        {
            result(i, 0) = Complex<T>{static_cast<T>(out(i, i)), 0};
        }
    }
    return result;
}

// Algorithm: Eigenvalue Verification
// Verifies computed eigenvalues by checking both the trace (sum of eigenvalues
// equals matrix trace) and determinant (product of eigenvalues equals matrix
// determinant) invariants.
template <typename T, Size R, Size C>
static inline constexpr bool checkEigenValues(
    const Matrix<T, R, C> a, const Matrix<Complex<T>, R, 1> lambda,
    const T thresh)
{
    T tr = trace(a);
    Complex<T> sum_lambda{};
    for (Size i = 0; i < R; ++i)
    {
        sum_lambda = sum_lambda + lambda(i, 0);
    }

    if (consteig::abs(sum_lambda.real - tr) > thresh)
    {
        return false;
    }
    if (consteig::abs(sum_lambda.imag) > thresh)
    {
        return false;
    }

    if constexpr (R <= 4)
    {
        T d = determinant(a);
        Complex<T> prod_lambda{1, 0};
        for (Size i = 0; i < R; ++i)
        {
            prod_lambda = prod_lambda * lambda(i, 0);
        }
        T det_tol = thresh * (static_cast<T>(1) + consteig::abs(d));
        if (consteig::abs(prod_lambda.real - d) > det_tol)
        {
            return false;
        }
        if (consteig::abs(prod_lambda.imag) > det_tol)
        {
            return false;
        }
    }

    return true;
}

// Algorithm: Inverse Iteration
// Computes the eigenvectors of a matrix A given its eigenvalues.
// Solves (A - \lambda I)v = b iteratively to find the eigenvector v.
template <typename T, Size S>
constexpr Matrix<Complex<T>, S, S> eigenvectors(
    const Matrix<T, S, S> &A, const Matrix<Complex<T>, S, 1> &eigenvalues)
{
    Matrix<Complex<T>, S, S> V{};

    for (Size i = 0; i < S; ++i)
    {
        Complex<T> lambda = eigenvalues(i, 0);

        // Form shifted matrix: (A - \lambda I)
        Matrix<Complex<T>, S, S> shifted_A{};
        for (Size r = 0; r < S; ++r)
        {
            for (Size c = 0; c < S; ++c)
            {
                shifted_A(r, c) = Complex<T>{A(r, c), 0};
                if (r == c)
                {
                    shifted_A(r, c) = shifted_A(r, c) - lambda;
                }
            }
        }

        // LU decomposition of the shifted matrix
        LUMatrix<Complex<T>, S> lu_res = lu(shifted_A);

        // Initial random vector b
        // In a strict constexpr environment, we use a deterministic "random"
        // vector (e.g., all 1s).
        Matrix<Complex<T>, S, 1> b{};
        for (Size j = 0; j < S; ++j)
        {
            b(j, 0) = Complex<T>{1.0, 0.0};
        }

        // Inverse iteration (usually 1 or 2 iterations is sufficient for
        // convergence given that the eigenvalue is highly accurate).
        for (Size iter = 0; iter < 2; ++iter)
        {
            b = lu_solve(lu_res, b);

            // Safe normalization to prevent overflow during Euclidean norm
            // calculation. First, scale by the maximum absolute component.
            T max_val = 0;
            for (Size j = 0; j < S; ++j)
            {
                T abs_real = consteig::abs(b(j, 0).real);
                T abs_imag = consteig::abs(b(j, 0).imag);
                if (abs_real > max_val)
                {
                    max_val = abs_real;
                }
                if (abs_imag > max_val)
                {
                    max_val = abs_imag;
                }
            }

            if (max_val > 0)
            {
                T inv_max = static_cast<T>(1) / max_val;
                for (Size j = 0; j < S; ++j)
                {
                    b(j, 0) = b(j, 0) * inv_max;
                }
            }

            // Now compute Euclidean norm safely
            T norm_sq = 0;
            for (Size j = 0; j < S; ++j)
            {
                norm_sq = norm_sq + b(j, 0).real * b(j, 0).real +
                          b(j, 0).imag * b(j, 0).imag;
            }
            T norm = consteig::sqrt(norm_sq);

            if (norm > 0)
            {
                T inv_norm = static_cast<T>(1) / norm;
                for (Size j = 0; j < S; ++j)
                {
                    b(j, 0) = b(j, 0) * inv_norm;
                }
            }
        }

        // Store the computed eigenvector in the i-th column of V
        for (Size r = 0; r < S; ++r)
        {
            V(r, i) = b(r, 0);
        }
    }

    return V;
}

} // namespace consteig

#endif

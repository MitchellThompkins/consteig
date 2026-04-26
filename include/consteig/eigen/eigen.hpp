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

/// @defgroup eigen Eigensolvers
/// @brief Functions for computing eigenvalues and eigenvectors at compile time.
/// @{

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

/// @internal
/// Applies diagonal scaling only (Parlett & Reinsch 1969) to reduce the norm
/// of rows and columns. No permutation-based eigenvalue isolation is performed.
// Algorithm: Balancing
template <typename T, Size S>
constexpr Matrix<T, S, S> balance(Matrix<T, S, S> a)
{
    bool converged = false;
    T factor = static_cast<T>(2);

    for (Size iter = 0; iter < 10 && !converged; ++iter)
    {
        converged = true;
        for (Size row = 0; row < S; ++row)
        {
            T row_norm = 0;
            T col_norm = 0;
            for (Size col = 0; col < S; ++col)
            {
                if (row != col)
                {
                    row_norm += abs(a(row, col));
                    col_norm += abs(a(col, row));
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
                    for (Size col = 0; col < S; ++col)
                    {
                        a(row, col) *= f;
                    }
                    for (Size scale_row = 0; scale_row < S; ++scale_row)
                    {
                        a(scale_row, row) /= f;
                    }
                }
            }
        }
    }
    return a;
}

/// @internal
/// Wilkinson shift: default shifting strategy for QR iteration
/// (quadratically convergent in most cases).
// Algorithm: Wilkinson Shifts
template <typename T>
constexpr T wilkinsonShift(const T a, const T b, const T c)
{
    T delta{(a - c) / 2};
    if (delta == static_cast<T>(0))
    {
        delta = consteig::epsilon<T>();
    }
    T disc = delta * delta + b * b;
    T s = (delta < 0) ? -sqrt(disc) : sqrt(disc);
    return c - (b * b) / (delta + s);
}

/// @internal
/// Implicit double-shift Francis QR step with Householder bulge chasing.
// Algorithm: Implicit Double-Shift QR (Francis QR Step)
template <typename T, Size S>
constexpr void francis_qr_step(Matrix<T, S, S> &H, Size l, Size n, T s, T t)
{
    T p1 = H(l, l) * H(l, l) + H(l, l + 1) * H(l + 1, l) - s * H(l, l) + t;
    T p2 = H(l + 1, l) * (H(l, l) + H(l + 1, l + 1) - s);
    T p3 = (l + 2 <= n) ? H(l + 1, l) * H(l + 2, l + 1) : static_cast<T>(0);

    for (Size pos = l; pos < n; ++pos)
    {
        Size m = (pos + 2 <= n) ? 3 : 2;
        T v1 = 0, v2 = 0, v3 = 0, norm = 0;

        if (m == 3)
        {
            norm = sqrt(p1 * p1 + p2 * p2 + p3 * p3);
            v1 = p1 + (p1 < 0 ? -norm : norm);
            v2 = p2;
            v3 = p3;
        }
        else
        {
            norm = sqrt(p1 * p1 + p2 * p2);
            v1 = p1 + (p1 < 0 ? -norm : norm);
            v2 = p2;
            v3 = 0;
        }

        if (norm > 0)
        {
            T v_sum_sq = v1 * v1 + v2 * v2 + v3 * v3;
            T beta = static_cast<T>(2) / v_sum_sq;

            // Left application: include column pos-1 for pos > l to chase the
            // bulge
            Size col_start = (pos > l) ? pos - 1 : pos;
            for (Size col = col_start; col < S; ++col)
            {
                T sum = beta * (v1 * H(pos, col) + v2 * H(pos + 1, col) +
                                (m == 3 ? v3 * H(pos + 2, col) : 0));
                H(pos, col) -= sum * v1;
                H(pos + 1, col) -= sum * v2;
                if (m == 3)
                {
                    H(pos + 2, col) -= sum * v3;
                }
            }

            // Right application
            Size upper_row = (pos + 3 < n + 1) ? pos + 3 : n;
            for (Size row = 0; row <= upper_row && row < S; ++row)
            {
                T sum = beta * (v1 * H(row, pos) + v2 * H(row, pos + 1) +
                                (m == 3 ? v3 * H(row, pos + 2) : 0));
                H(row, pos) -= sum * v1;
                H(row, pos + 1) -= sum * v2;
                if (m == 3)
                {
                    H(row, pos + 2) -= sum * v3;
                }
            }

            // Explicitly zero bulge elements for numerical stability
            if (pos > l)
            {
                H(pos + 1, pos - 1) = 0;
                if (m == 3)
                {
                    H(pos + 2, pos - 1) = 0;
                }
            }
        }

        if (pos < n - 1)
        {
            p1 = H(pos + 1, pos);
            p2 = H(pos + 2, pos);
            if (pos < n - 2)
            {
                p3 = H(pos + 3, pos);
            }
        }
    }
}

/// @internal
/// Real arithmetic implicit double-shift QR. Handles complex conjugate pairs
/// simultaneously via 2×2 bulge chasing, avoiding complex arithmetic in the
/// iteration loop. A single-shift complex QR would resolve clustered conjugate
/// pairs more cleanly but would roughly quadruple arithmetic in the inner loop.
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
            T diagonal_sum = abs(a(l, l)) + abs(a(l - 1, l - 1));
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
            if (abs(a(l, l - 1)) <= eps * diagonal_sum ||
                abs(a(l, l - 1)) <= eps)
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
                sshift = abs(a(n, n - 1)) + abs(a(n - 1, n - 2));
            }
            else
            {
                // Top-based exceptional shift
                sshift = abs(a(l + 1, l));
                if (l + 2 <= n)
                {
                    sshift += abs(a(l + 2, l + 1));
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

/// @internal
/// Single-shift QR iteration for symmetric (real eigenvalue) matrices.
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
        if (abs(a(n - 1, n - 2)) <=
            eps * (abs(a(n - 1, n - 1)) + abs(a(n - 2, n - 2))))
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

/// @brief Reduce a matrix to quasi-upper-triangular (Schur) form.
///
/// Routes to the single-shift or double-shift QR solver based on the
/// symmetry of `a` (controlled by `symmetryTolerance`). The returned
/// matrix is in real Schur form: diagonal entries are the real eigenvalues,
/// and 2×2 diagonal blocks encode complex conjugate pairs.
///
/// In most cases you want @ref eigenvalues or @ref eigenvectors rather than
/// this function directly.
///
/// @tparam T  Floating-point scalar type.
/// @tparam S  Matrix dimension.
/// @param  a                  Real square matrix.
/// @param  symmetryTolerance  Threshold for routing to the symmetric solver.
///                            Default: @ref
///                            CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE.
/// @return Quasi-upper-triangular matrix in real Schur form.
/// @pre `T` must be a floating-point type (enforced by `static_assert`).
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

/// @brief Compute the eigenvalues of a real square matrix.
///
/// Returns the complete spectrum as a column vector of @ref Complex values.
/// Real eigenvalues have zero imaginary part; complex eigenvalues appear as
/// conjugate pairs.
///
/// Internally promotes to `double` (or `long double` if
/// @ref CONSTEIG_USE_LONG_DOUBLE is defined) for better numerical stability,
/// then narrows back to `T` for the result.
///
/// @tparam T  Floating-point scalar type of the input matrix.
/// @tparam S  Matrix dimension.
/// @param  a  Real S×S matrix.
/// @return S×1 column vector of eigenvalues as @ref Complex<T>.
///         No particular ordering is guaranteed.
///
/// @note Accuracy on well-conditioned matrices: ~1e-9.
///       Accuracy on defective matrices (Jordan blocks): ~0.03.
///       See the verification documentation for details.
///
/// @code
/// static constexpr consteig::Matrix<double, 2, 2> A{{
///     {0.0, -1.0},
///     {1.0,  0.0}
/// }};
/// static constexpr auto eigs = consteig::eigenvalues(A);
/// // eigs(0,0) ≈ Complex{0.0,  1.0}
/// // eigs(1,0) ≈ Complex{0.0, -1.0}
/// @endcode
template <typename T, Size S>
constexpr Matrix<Complex<T>, S, 1> eigenvalues(const Matrix<T, S, S> &a)
{
    static_assert(is_float<T>(), "eigenvalues expects floating point type");
    Matrix<InternalScalar, S, S> a_internal{};
    for (Size row = 0; row < S; ++row)
    {
        for (Size col = 0; col < S; ++col)
        {
            a_internal(row, col) = static_cast<InternalScalar>(a(row, col));
        }
    }

    Matrix<InternalScalar, S, S> out = eig(a_internal);
    Matrix<Complex<T>, S, 1> result{};
    InternalScalar eps = consteig::epsilon<InternalScalar>() *
                         (norm1(out) + static_cast<InternalScalar>(1.0));

    for (Size diag = 0; diag < S; ++diag)
    {
        // If subdiag is essentially zero (smaller than eps), we have a 1x1 *
        // block.  If subdiag is significantly larger than zero, it means the
        // current row and the next row are "tangled" together, forming a 2x2
        // block (which means there should be complex conjugate eigen values.
        bool found_2x2 = false;
        if (diag < S - 1)
        {
            InternalScalar subdiag = out(diag + 1, diag);
            if (abs(subdiag) > eps)
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
            InternalScalar a00 = out(diag, diag);
            InternalScalar a01 = out(diag, diag + 1);
            InternalScalar a10 = out(diag + 1, diag);
            InternalScalar a11 = out(diag + 1, diag + 1);
            InternalScalar tr = a00 + a11;
            InternalScalar d = a00 * a11 - a01 * a10;
            InternalScalar disc = tr * tr - 4 * d;
            if (disc >= 0)
            {
                InternalScalar sq = sqrt(disc);
                result(diag, 0) = Complex<T>{static_cast<T>((tr + sq) / 2), 0};
                result(diag + 1, 0) =
                    Complex<T>{static_cast<T>((tr - sq) / 2), 0};
            }
            else
            {
                InternalScalar sq = sqrt(-disc);
                result(diag, 0) =
                    Complex<T>{static_cast<T>(tr / 2), static_cast<T>(sq / 2)};
                result(diag + 1, 0) =
                    Complex<T>{static_cast<T>(tr / 2), static_cast<T>(-sq / 2)};
            }
            diag++;
        }
        else
        {
            result(diag, 0) = Complex<T>{static_cast<T>(out(diag, diag)), 0};
        }
    }
    return result;
}

/// @brief Verify computed eigenvalues against matrix invariants.
///
/// Checks two matrix invariants:
/// 1. **Trace**: sum of eigenvalues must equal the matrix trace within
/// `thresh`.
/// 2. **Determinant** (only for `R <= 4`): product of eigenvalues must equal
///    `det(a)` within a scaled tolerance.
///
/// Use this in `static_assert` blocks to validate eigenvalue correctness at
/// compile time:
///
/// @code
/// static constexpr auto eigs = consteig::eigenvalues(A);
/// static_assert(consteig::checkEigenValues(A, eigs, 1e-9), "Bad eigenvalues");
/// @endcode
///
/// @tparam T  Floating-point scalar type.
/// @tparam R  Number of rows (must equal `C`).
/// @tparam C  Number of columns.
/// @param  a       Input matrix.
/// @param  lambda  Eigenvalue column vector from @ref eigenvalues.
/// @param  thresh  Absolute tolerance for invariant checks.
/// @return `true` if both invariants hold within `thresh`.
// Algorithm: Eigenvalue Verification
template <typename T, Size R, Size C>
static inline constexpr bool checkEigenValues(
    const Matrix<T, R, C> &a, const Matrix<Complex<T>, R, 1> &lambda,
    const T thresh)
{
    T tr = trace(a);
    Complex<T> sum_lambda{};
    for (Size row = 0; row < R; ++row)
    {
        sum_lambda = sum_lambda + lambda(row, 0);
    }

    if (abs(sum_lambda.real - tr) > thresh)
    {
        return false;
    }
    if (abs(sum_lambda.imag) > thresh)
    {
        return false;
    }

    if constexpr (R <= 4)
    {
        T d = determinant(a);
        Complex<T> prod_lambda{1, 0};
        for (Size row = 0; row < R; ++row)
        {
            prod_lambda = prod_lambda * lambda(row, 0);
        }
        T det_tol = thresh * (static_cast<T>(1) + abs(d));
        if (abs(prod_lambda.real - d) > det_tol)
        {
            return false;
        }
        if (abs(prod_lambda.imag) > det_tol)
        {
            return false;
        }
    }

    return true;
}

/// @brief Compute eigenvectors given a matrix and its eigenvalues.
///
/// Uses inverse iteration: for each eigenvalue λ, solves (A - λI)v = b
/// for a normalized vector v. Two iterations are performed per eigenvector,
/// which is typically sufficient given the accuracy of @ref eigenvalues.
///
/// Each column of the returned matrix is the eigenvector corresponding to
/// the eigenvalue at the same index in `eigenvalues`.
///
/// @tparam T  Floating-point scalar type.
/// @tparam S  Matrix dimension.
/// @param  A            Real S×S matrix.
/// @param  eigenvalues  S×1 eigenvalue vector from @ref eigenvalues.
/// @return S×S matrix whose columns are the eigenvectors (as @ref Complex<T>).
///         Column `eig_col` corresponds to `eigenvalues(eig_col,0)`.
///
/// @code
/// static constexpr auto eigs = consteig::eigenvalues(A);
/// static constexpr auto vecs = consteig::eigenvectors(A, eigs);
/// // vecs.col(0) is the eigenvector for eigs(0,0)
/// @endcode
// Algorithm: Inverse Iteration
template <typename T, Size S>
constexpr Matrix<Complex<T>, S, S> eigenvectors(
    const Matrix<T, S, S> &A, const Matrix<Complex<T>, S, 1> &eigenvalues)
{
    static_assert(is_float<T>(), "eigenvectors expects floating point type");
    Matrix<Complex<T>, S, S> V{};

    for (Size eig_col = 0; eig_col < S; ++eig_col)
    {
        Complex<T> lambda = eigenvalues(eig_col, 0);

        // Form shifted matrix: (A - \lambda I)
        Matrix<Complex<T>, S, S> shifted_A{};
        for (Size row = 0; row < S; ++row)
        {
            for (Size col = 0; col < S; ++col)
            {
                shifted_A(row, col) = Complex<T>{A(row, col), 0};
                if (row == col)
                {
                    shifted_A(row, col) = shifted_A(row, col) - lambda;
                }
            }
        }

        // LU decomposition of the shifted matrix
        LUMatrix<Complex<T>, S> lu_res = lu(shifted_A);

        // Initial random vector b
        // In a strict constexpr environment, we use a deterministic "random"
        // vector (e.g., all 1s).
        Matrix<Complex<T>, S, 1> b{};
        for (Size row = 0; row < S; ++row)
        {
            b(row, 0) = Complex<T>{1.0, 0.0};
        }

        // Inverse iteration (usually 1 or 2 iterations is sufficient for
        // convergence given that the eigenvalue is highly accurate).
        for (Size iter = 0; iter < 2; ++iter)
        {
            b = lu_solve(lu_res, b);

            // Safe normalization to prevent overflow during Euclidean norm
            // calculation. First, scale by the maximum absolute component.
            T max_val = 0;
            for (Size row = 0; row < S; ++row)
            {
                T abs_real = abs(b(row, 0).real);
                T abs_imag = abs(b(row, 0).imag);
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
                for (Size row = 0; row < S; ++row)
                {
                    b(row, 0) = b(row, 0) * inv_max;
                }
            }

            // Now compute Euclidean norm safely
            T norm_sq = 0;
            for (Size row = 0; row < S; ++row)
            {
                norm_sq = norm_sq + b(row, 0).real * b(row, 0).real +
                          b(row, 0).imag * b(row, 0).imag;
            }
            T norm = sqrt(norm_sq);

            if (norm > 0)
            {
                T inv_norm = static_cast<T>(1) / norm;
                for (Size row = 0; row < S; ++row)
                {
                    b(row, 0) = b(row, 0) * inv_norm;
                }
            }
        }

        // Store the computed eigenvector in the eig_col-th column of V
        for (Size row = 0; row < S; ++row)
        {
            V(row, eig_col) = b(row, 0);
        }
    }

    return V;
}

/// @brief Convenience class that computes both eigenvalues and eigenvectors.
///
/// Constructs both at initialization time. Prefer the free functions
/// @ref eigenvalues and @ref eigenvectors when you only need one.
///
/// @tparam T  Floating-point scalar type.
/// @tparam S  Matrix dimension.
///
/// @code
/// static constexpr consteig::EigenSolver<double, 3> solver(A);
/// static constexpr auto eigs = solver.eigenvalues();
/// static constexpr auto vecs = solver.eigenvectors();
/// @endcode
template <typename T, Size S> class EigenSolver
{
  public:
    /// @brief Compute eigenvalues and eigenvectors of `mat`.
    constexpr EigenSolver(const Matrix<T, S, S> &mat)
        : _evals(consteig::eigenvalues(mat)),
          _evecs(consteig::eigenvectors(mat, _evals))
    {
    }

    /// @brief Return the S×1 eigenvalue column vector.
    constexpr const Matrix<Complex<T>, S, 1> &eigenvalues() const
    {
        return _evals;
    }

    /// @brief Return the S×S eigenvector matrix (column `eig_col` corresponds
    /// to eigenvalue `eig_col`).
    constexpr const Matrix<Complex<T>, S, S> &eigenvectors() const
    {
        return _evecs;
    }

  private:
    Matrix<Complex<T>, S, 1> _evals;
    Matrix<Complex<T>, S, S> _evecs;
};

/// @}  // defgroup eigen

} // namespace consteig

#endif

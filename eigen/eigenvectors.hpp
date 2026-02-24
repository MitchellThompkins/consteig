#ifndef EIGENVECTORS_HPP
#define EIGENVECTORS_HPP

#include "../math/complex.hpp"
#include "../math/constmath.hpp"
#include "../matrix/decompositions/lu.hpp"
#include "../matrix/matrix.hpp"

namespace consteig
{

// Algorithm: Inverse Iteration
// Computes the eigenvectors of a matrix A given its eigenvalues.
// Solves (A - \lambda I)v = b iteratively to find the eigenvector v.
template <typename T, Size S>
constexpr Matrix<Complex<T>, S, S> eigvecs(
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
        // In a strict constexpr environment, we use a deterministic "random" vector (e.g., all 1s).
        Matrix<Complex<T>, S, 1> b{};
        for (Size j = 0; j < S; ++j)
        {
            b(j, 0) = Complex<T>{1.0, 0.0};
        }

        // Inverse iteration (usually 1 or 2 iterations is sufficient for convergence
        // given that the eigenvalue is highly accurate).
        for (Size iter = 0; iter < 2; ++iter)
        {
            b = lu_solve(lu_res, b);

            // Safe normalization to prevent overflow during Euclidean norm calculation.
            // First, scale by the maximum absolute component.
            T max_val = 0;
            for (Size j = 0; j < S; ++j)
            {
                T abs_real = consteig::abs(b(j, 0).real);
                T abs_imag = consteig::abs(b(j, 0).imag);
                if (abs_real > max_val) max_val = abs_real;
                if (abs_imag > max_val) max_val = abs_imag;
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

#endif // EIGENVECTORS_HPP

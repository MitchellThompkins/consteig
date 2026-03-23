#ifndef HESSENBERG_HPP
#define HESSENBERG_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"
#include "householder.hpp"

namespace consteig
{

///////////// TYPES /////////////
template <typename T, Size S> struct PHMatrix
{
    Matrix<T, S, S> _p;
    Matrix<T, S, S> _h;

    constexpr PHMatrix() = default;
    constexpr PHMatrix(const PHMatrix &) = default;
    constexpr PHMatrix(PHMatrix &&) = default;
    constexpr PHMatrix &operator=(const PHMatrix &) = default;
    constexpr PHMatrix &operator=(PHMatrix &&) = default;
};

///////////// FUNCTION DECLARATIONS /////////////
template <typename T, Size R, Size C, Size L = R>
constexpr PHMatrix<T, R> hess(Matrix<T, R, C> a);

///////////// IMPLEMENTATIONS /////////////

// Algorithm: Hessenberg Reduction
// Reduces a square matrix A to upper Hessenberg form H via orthogonal
// similarity: H = P^T * A * P, where P is the accumulated product of
// Householder reflectors. H has zeros below the first subdiagonal.
//
// Reducing to Hessenberg form before QR iteration cuts the cost of each
// QR step from O(n^3) to O(n^2), making the overall solver O(n^3).
//
// The reduction is implemented recursively. At each level, a Householder
// reflector P is computed from the trailing submatrix and applied as a
// similarity transformation P^T * A * P, preserving eigenvalues. The template
// parameter L tracks the remaining submatrix size, bottoming out at L <= 2
// when no further reduction is needed.
//
// Reference: Golub & Van Loan, "Matrix Computations" (4th ed.), sec. 7.4
template <typename T, Size R, Size C, Size L>
constexpr PHMatrix<T, R> hess(Matrix<T, R, C> a)
{
    static_assert(is_float<T>(), "hess expects floating point");
    static_assert(R == C, "Hessenberg reduction expects a square matrix");

    if constexpr (L <= 2)
    {
        // Base case: submatrix is 2x2 or smaller, already Hessenberg
        PHMatrix<T, R> result{};
        result._h = a;
        return result;
    }
    else
    {
        constexpr Size size{R};
        constexpr Size houseSize{L};

        // Extract the trailing submatrix and compute its Householder reflector
        Matrix<T, L, L> subA{a.template block<R - houseSize, R - houseSize,
                                              houseSize, houseSize>()};
        Matrix<T, L, L> m{house(subA)};

        // Embed the reflector into a full-size identity matrix
        Matrix<T, size, size> p{eye<T, R>()};
        p.template setBlock<R - houseSize + 1, R - houseSize + 1, houseSize - 1,
                            houseSize - 1>(
            m.template block<1, 1, houseSize - 1, houseSize - 1>());

        // Apply the similarity transformation P * A * P and recurse
        PHMatrix<T, R> out = hess<T, R, R, houseSize - 1>(p * a * p);

        // Accumulate the orthogonal factor P
        Matrix<T, size, size> pRtn = (houseSize > 3) ? p * out._p : p;

        return {pRtn, out._h};
    }
}

} // namespace consteig
#endif

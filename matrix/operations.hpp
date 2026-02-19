#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "matrix.hpp"

namespace consteig {

/*
 * | Operation             | Operator    |
 * |-----------------------+-------------|
 * | Addition              | +           |
 * | Subtraction           | -           |
 * | Matrix Multiplication | *           |
 * | Scalar Multiplication | *           |
 * | Dot Product           | dot()       |
 * | Transpose             | transpose() |
 * | Create Diagonal       | diagonal()  |
 * | Create Identity       | eye()       |
 * | Euclidean Normal      | normE()     |
 * | Square Root           | sqrt()      |
 * | Determinant           | det()       |
 */

// https://pages.mtu.edu/~struther/Courses/OLD/Other/Sp2012/5627/BlockQR/Work/MA5629%20presentation.pdf
///////////// IMPLEMENTATIONS /////////////

template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> operator+(const Matrix<T, R, C> &lhs, const Matrix<T, R, C> &rhs) {
    Matrix<T, R, C> result{};

    for (Size i{0}; i < R; ++i)
        for (Size j{0}; j < C; ++j) result(i, j) = lhs(i, j) + rhs(i, j);

    return result;
}

template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> operator-(const Matrix<T, R, C> &lhs, const Matrix<T, R, C> &rhs) {
    Matrix<T, R, C> result{};

    for (Size i{0}; i < R; ++i)
        for (Size j{0}; j < C; ++j) result(i, j) = lhs(i, j) - rhs(i, j);

    return result;
}

// Multiply two matrices
template <typename T, Size R1, Size C1, Size R2, Size C2>
constexpr Matrix<T, R1, C2> operator*(const Matrix<T, R1, C1> &lhs, const Matrix<T, R2, C2> &rhs) {
    static_assert(C1 == R2, "Number of columns must equal number of rows");
    Matrix<T, R1, C2> result{};

    for (Size i{0}; i < R1; i++)
        for (Size j{0}; j < C2; j++)
            for (Size k{0}; k < C1; k++) result(i, j) += lhs(i, k) * rhs(k, j);

    return result;
}

// Multiply by a scalar
// todo(mthompkins): Figure out how to not make it possible to pass the scalar
// to either side
template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> operator*(const T &lhs, const Matrix<T, R, C> &rhs) {
    Matrix<T, R, C> result{};

    for (Size i{0}; i < R; i++)
        for (Size j{0}; j < C; j++) result(i, j) = lhs * rhs(i, j);

    return result;
}

// Multiply a 1XN by a Nx1 matrix
template <typename T, Size R, Size C>
constexpr T dot(const Matrix<T, R, C> &lhs, const Matrix<T, R, C> &rhs) {
    static_assert(R == 1, "Dot Product expects two 1xN matrices");
    Matrix<T, 1, 1> product{lhs * transpose(rhs)};
    T result{product(0, 0)};

    return result;
}

template <typename T, Size R, Size C>
constexpr Matrix<T, C, R> transpose(const Matrix<T, R, C> &mat) {
    Matrix<T, C, R> result{};

    for (Size i{0}; i < R; i++)
        for (Size j{0}; j < C; j++) result(j, i) = mat(i, j);

    return result;
}

template <typename T, Size S>
constexpr Matrix<T, S, S> diagonal(const T val) {
    Matrix<T, S, S> result{};

    for (Size i{0}, j{0}; i < S; i++, j++) result(i, j) = val;

    return result;
}

template <typename T, Size S>
constexpr Matrix<T, S, S> eye() {
    return diagonal<T, S>(static_cast<T>(1));
}

// Euclidean normal of a matrix
template <typename T, Size R, Size C>
constexpr T normE(const Matrix<T, R, C> &mat) {
    T result{};

    for (Size i{0}; i < R; i++)
        for (Size j{0}; j < C; j++) result += (mat(i, j) * mat(i, j));

    return consteig::sqrt(result);
}

// 1-norm of a matrix (max column sum)
template <typename T, Size R, Size C>
constexpr T norm1(const Matrix<T, R, C> &mat) {
    T max_sum{static_cast<T>(0)};
    for (Size j{0}; j < C; ++j) {
        T col_sum{static_cast<T>(0)};
        for (Size i{0}; i < R; ++i) {
            col_sum += consteig::abs(mat(i, j));
        }
        if (col_sum > max_sum) max_sum = col_sum;
    }
    return max_sum;
}

// Infinity-norm of a matrix (max row sum)
template <typename T, Size R, Size C>
constexpr T normInf(const Matrix<T, R, C> &mat) {
    T max_sum{static_cast<T>(0)};
    for (Size i{0}; i < R; ++i) {
        T row_sum{static_cast<T>(0)};
        for (Size j{0}; j < C; ++j) {
            row_sum += consteig::abs(mat(i, j));
        }
        if (row_sum > max_sum) max_sum = row_sum;
    }
    return max_sum;
}

template <typename T, Size R, Size C>
constexpr Matrix<T, R, C> sqrt(const Matrix<T, R, C> &mat) {
    T result{};

    for (Size i{0}; i < R; i++)
        for (Size j{0}; j < C; j++) result(i, j) = consteig::sqrt(mat(i, j));

    return result;
}

// Forward declaration or include for eigvals if needed.
// Since operations.hpp is usually included before eigen.hpp in consteig.hpp,
// we might need to handle this carefully.
// Actually, consteig.hpp includes eigen.hpp last.
// Let's use a simpler approach for det() that doesn't depend on eigen.hpp if possible,
// or just accept that it's O(n!) for now if it's not hurting anyone.
// But wait, QR is in decompositions/qr.hpp which is included in decompositions.hpp.
// Let's use the R diagonal product from QR.

template <typename T, Size R, Size C>
constexpr T det(const Matrix<T, R, C> &mat) {
    static_assert(R == C, "Can only find determinant of a square matrix");

    if constexpr (R == 1) {
        return mat(0, 0);
    } else if constexpr (R == 2) {
        return (mat(0, 0) * mat(1, 1)) - (mat(0, 1) * mat(1, 0));
    } else {
        // Use recursive expansion for now to avoid circular dependency with eigen.hpp
        // but it's already there.
        // Let's stick with the recursive one if it's not a bottleneck,
        // or implement a simple Gaussian elimination based one.

        T result{static_cast<T>(0)};
        for (Size i{0}; i < R; i++) {
            Matrix<T, R - 1, C - 1> submat{};
            for (Size j{1}; j < R; j++) {
                Size subj{0U};
                for (Size k{0}; k < R; k++) {
                    if (k == i) continue;
                    submat(j - 1, subj) = mat(j, k);
                    subj++;
                }
            }
            T sign = (i % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
            result += (sign * mat(0, i) * det(submat));
        }
        return result;
    }
}

template <typename T>
constexpr T det(const Matrix<T, 2, 2> &mat) {
    return (mat(0, 0) * mat(1, 1)) - (mat(0, 1) * mat(1, 0));
}

template <typename T, Size R, Size C>
constexpr T trace(const Matrix<T, R, C> &mat) {
    static_assert(R == C, "Trace expects a square matrix");
    T result{static_cast<T>(0)};
    for (Size i{0}; i < R; ++i) result += mat(i, i);
    return result;
}

}  // namespace consteig
#endif

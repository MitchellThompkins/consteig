#ifndef HESSENBERG_HPP
#define HESSENBERG_HPP

#include "../../math/constmath.hpp"
#include "../matrix.hpp"
#include "../operations.hpp"
#include "householder.hpp"

namespace consteig {

///////////// TYPES /////////////
template <typename T, Size S>
struct PHMatrix {
    Matrix<T, S, S> _p;
    Matrix<T, S, S> _h;

    constexpr PHMatrix() = default;
    constexpr PHMatrix(const PHMatrix&) = default;
    constexpr PHMatrix(PHMatrix&&) = default;
    constexpr PHMatrix& operator=(const PHMatrix&) = default;
    constexpr PHMatrix& operator=(PHMatrix&&) = default;
};

///////////// FUNCTION DECLARATIONS /////////////
template <typename T, Size R, Size C, Size L = R>
constexpr PHMatrix<T, R> hess(Matrix<T, R, C> a);

///////////// IMPLEMENTATIONS /////////////

template <typename T, Size R, Size C, Size L>
constexpr PHMatrix<T, R> hess(Matrix<T, R, C> a) {
    static_assert(is_float<T>(), "hess expects floating point");
    static_assert(R == C, "Hessenberg reduction expects a square matrix");

    if constexpr (L <= 2) {
        PHMatrix<T, R> result{};
        result._h = a;
        return result;
    } else {
        constexpr Size size{R};
        constexpr Size houseSize{L};
        constexpr Size end{R - 1};

        Matrix<T, L, L> subA{a.template sub<R - houseSize, R - houseSize, end, end>()};
        Matrix<T, L, L> m{house(subA)};

        Matrix<T, size, size> p{eye<T, R>()};
        p.template setSub<R - houseSize + 1, R - houseSize + 1, end, end>(
            m.template sub<1, 1, houseSize - 1, houseSize - 1>());

        PHMatrix<T, R> out = hess<T, R, R, houseSize - 1>(p * a * p);

        Matrix<T, size, size> pRtn = (houseSize > 3) ? p * out._p : p;

        return {pRtn, out._h};
    }
}

}  // namespace consteig
#endif

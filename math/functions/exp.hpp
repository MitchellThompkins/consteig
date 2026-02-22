#ifndef CONSTMATH_EXP_HPP
#define CONSTMATH_EXP_HPP

#include "../complex.hpp"
#include "pow.hpp"
#include "utilities.hpp"

namespace consteig {

// Max iterations for continued fraction expansion
constexpr int EXP_MAX_ITER = 50;

template <typename T>
constexpr T exp_cf_recur(const T x, const int depth_end) noexcept {
    int depth = EXP_MAX_ITER - 1;
    T res = static_cast<T>(1);
    while (depth > depth_end - 1) {
        res = static_cast<T>(1) + x / static_cast<T>(depth - 1) -
              x / static_cast<T>(depth) / res;
        --depth;
    }
    return res;
}

template <typename T>
constexpr T exp_cf(const T x) noexcept {
    return static_cast<T>(1) / (static_cast<T>(1) - x / exp_cf_recur(x, 2));
}

template <typename T>
constexpr T find_whole(const T x) noexcept {
    return static_cast<T>(static_cast<long long>(x));
}

template <typename T>
constexpr T find_fraction(const T x) noexcept {
    return x - find_whole(x);
}

template <typename T>
constexpr T exp_split(const T x) noexcept {
    return consteig::pow(static_cast<T>(E_CONST),
                         static_cast<int>(find_whole(x))) *
           exp_cf(find_fraction(x));
}

template <typename T>
constexpr T exp_check(const T x) noexcept {
    return (x < static_cast<T>(0)   ? static_cast<T>(1) / exp_check(-x)
            : x < static_cast<T>(2) ? exp_cf(x)
                                    : exp_split(x));
}

/**
 * @brief Computes the exponential of a real number.
 */
template <typename T>
constexpr auto exp(const T x) noexcept {
    if constexpr (!is_float<T>()) {
        return exp_check(static_cast<double>(x));
    } else {
        return exp_check(x);
    }
}

/**
 * @brief Computes the exponential of a complex number.
 * exp(z) = exp(x + iy) = exp(x) * (cos(y) + i*sin(y))
 */
template <typename T>
constexpr Complex<T> exp(const Complex<T>& z) noexcept {
    // We use the identity exp(z) = exp(x) * exp(iy)
    // exp(x) uses the real version, exp(iy) uses the continued fraction
    return exp(z.real) * exp_cf(Complex<T>(0, z.imag));
}

}  // namespace consteig

#endif

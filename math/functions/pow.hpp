#ifndef CONSTMATH_POW_HPP
#define CONSTMATH_POW_HPP

namespace consteig {

template <typename T>
constexpr T powi(const T x, const unsigned int n) {
    return n == 0 ? static_cast<T>(1) : n % 2 == 0 ? powi(x * x, n / 2) : powi(x * x, (n - 1) / 2) * x;
}

template <typename T>
constexpr T powi(T x, int n) {
    return n < 0 ? static_cast<T>(1) / powi(x, unsigned(-n)) : powi(x, unsigned(n));
}

}  // namespace consteig

#endif

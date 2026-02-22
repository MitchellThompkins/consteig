#ifndef CONSTMATH_POW_HPP
#define CONSTMATH_POW_HPP

namespace consteig {

// Exponentiation by squaring
template <typename T>
constexpr T powi(const T x, const unsigned int n) {
    return n == 0       ? static_cast<T>(1)
           : n % 2 == 0 ? powi(x * x, n / 2)
                        : powi(x * x, (n - 1) / 2) * x;
}

template <typename T>
constexpr T powi(const T x, const int n) {
    return n < 0 ? static_cast<T>(1) / powi(x, static_cast<unsigned int>(-n))
                 : powi(x, static_cast<unsigned int>(n));
}

}  // namespace consteig

#endif

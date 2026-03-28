#ifndef CONSTMATH_POW_HPP
#define CONSTMATH_POW_HPP

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Raise `x` to an unsigned integer power via exponentiation by
/// squaring.
///
/// @tparam T  Numeric type.
/// @param  x  Base value.
/// @param  n  Non-negative exponent.
/// @return `x^n`.
template <typename T> constexpr T pow(const T x, const unsigned int n)
{
    return n == 0       ? static_cast<T>(1)
           : n % 2 == 0 ? pow(x * x, n / 2)
                        : pow(x * x, (n - 1) / 2) * x;
}

/// @brief Raise `x` to a signed integer power.
///
/// Negative exponents compute `1 / x^|n|`.
///
/// @tparam T  Numeric type.
/// @param  x  Base value.
/// @param  n  Integer exponent (may be negative).
/// @return `x^n`.
template <typename T> constexpr T pow(const T x, const int n)
{
    return n < 0 ? static_cast<T>(1) / pow(x, static_cast<unsigned int>(-n))
                 : pow(x, static_cast<unsigned int>(n));
}

/// @}  // addtogroup math

} // namespace consteig

#endif

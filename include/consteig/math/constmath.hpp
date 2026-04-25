#ifndef CONSTMATH_HPP
#define CONSTMATH_HPP

#include "complex.hpp"
#include "functions/csqrt.hpp"
#include "math_backend.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Absolute value (modulus) of a complex number: `sqrt(re^2 + im^2)`.
/// @tparam T  Floating-point element type.
/// @param  c  Input complex number.
/// @return Non-negative real magnitude.
template <typename T> constexpr T abs(const Complex<T> &c)
{
    return sqrt(c.real * c.real + c.imag * c.imag);
}

/// @brief Exponential of a complex number using Euler's formula.
///
/// exp(x + iy) = exp(x) * (cos(y) + i*sin(y))
///
/// @tparam T  Floating-point element type.
/// @param  z  Input complex number.
template <typename T> constexpr Complex<T> exp(const Complex<T> &z) noexcept
{
    const T ex = exp(z.real);
    return {ex * cos(z.imag), ex * sin(z.imag)};
}

/// @}  // addtogroup math

} // namespace consteig

#endif

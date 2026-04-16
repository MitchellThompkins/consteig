#ifndef CONSTMATH_COMPLEX_EXP_HPP
#define CONSTMATH_COMPLEX_EXP_HPP

#include "../complex.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Exponential of a complex number using Euler's formula.
///
/// exp(x + iy) = exp(x) * (cos(y) + i*sin(y))
///
/// Requires exp(T), cos(T), and sin(T) to be declared in namespace consteig
/// before this header is included. constmath.hpp guarantees this for both
/// the built-in and gcem backends.
template <typename T> constexpr Complex<T> exp(const Complex<T> &z) noexcept
{
    const T ex = exp(z.real);
    return {ex * cos(z.imag), ex * sin(z.imag)};
}

/// @}  // addtogroup math

} // namespace consteig

#endif

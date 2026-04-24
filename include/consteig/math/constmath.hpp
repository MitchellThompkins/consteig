#ifndef CONSTMATH_HPP
#define CONSTMATH_HPP

#include "../consteig_options.hpp"

#ifdef CONSTEIG_USE_GCEM

// Configure gcem's type-traits mode BEFORE including any gcem header.
// Without CONSTEIG_GCEM_USE_STDLIB, gcem uses compiler builtins only
// (freestanding). With it, gcem uses <limits> and <type_traits>.
#ifndef CONSTEIG_GCEM_USE_STDLIB
#define GCEM_TRAITS_BUILTIN
#endif

#include "../optional_dependencies/gcem/gcem.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

using gcem::abs;
using gcem::cos;
using gcem::exp;
using gcem::pow;
using gcem::sgn;
using gcem::sin;
using gcem::sqrt;
using gcem::tan;

/// @}  // addtogroup math

} // namespace consteig

#else

#include "functions/abs.hpp"
#include "functions/exp.hpp"
#include "functions/pow.hpp"
#include "functions/sgn.hpp"
#include "functions/sqrt.hpp"
#include "functions/trig.hpp"

#endif

#include "complex.hpp"
#include "functions/csqrt.hpp"
#include "functions/utilities.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Compare two scalar values within an absolute tolerance.
///
/// Returns `true` if `|a - b| < thresh`. Does not use relative tolerance,
/// so be careful when comparing values with very different magnitudes.
///
/// @tparam T      Type of the values being compared.
/// @tparam U      Type of the threshold (converted to `T` internally).
/// @param  a      First value.
/// @param  b      Second value.
/// @param  thresh Absolute tolerance.
/// @return `true` if the values are within `thresh` of each other.
template <typename T, typename U> constexpr bool equalWithin(T a, T b, U thresh)
{
    return abs(a - b) < static_cast<T>(thresh);
}

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

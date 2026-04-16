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

#include "complex.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Exponential of a complex number using Euler's formula.
///
/// exp(x + iy) = exp(x) * (cos(y) + i*sin(y))
template <typename T>
constexpr Complex<T> exp(const Complex<T> &z) noexcept
{
    const T ex = exp(z.real);
    return {ex * cos(z.imag), ex * sin(z.imag)};
}

/// @}  // addtogroup math

} // namespace consteig

#else

#include "functions/abs.hpp"
#include "functions/exp.hpp"
#include "functions/pow.hpp"
#include "functions/sgn.hpp"
#include "functions/sqrt.hpp"
#include "functions/trig.hpp"
#include "complex.hpp"

#endif

#include "functions/csqrt.hpp"
#include "functions/utilities.hpp"

#endif

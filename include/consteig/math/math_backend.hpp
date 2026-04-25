#ifndef CONSTEIG_MATH_BACKEND_HPP
#define CONSTEIG_MATH_BACKEND_HPP

#include "../consteig_options.hpp"
#include "functions/utilities.hpp"

#ifdef CONSTEIG_USE_GCEM

// Configure gcem's type-traits mode BEFORE including any gcem header.
// Default: freestanding builtin traits (no stdlib required).
// Opt-in: define CONSTEIG_GCEM_USE_STDLIB to use hosted <limits>/<type_traits>.
// Opt-in: define CONSTEIG_GCEM_USE_CUSTOM_TRAITS to supply your own trait
//         definitions in namespace gcem before including consteig headers.
// NOTE: the mode must be uniform across all translation units (ODR).
#ifdef CONSTEIG_GCEM_USE_STDLIB
// hosted stdlib mode — no macro needed, gcem defaults to this
#elif defined(CONSTEIG_GCEM_USE_CUSTOM_TRAITS)
#define GCEM_TRAITS_CUSTOM
#else
#define GCEM_TRAITS_BUILTIN
#endif

#include "../optional_dependencies/gcem/gcem.hpp"

#undef GCEM_TRAITS_BUILTIN
#undef GCEM_TRAITS_CUSTOM

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

/// @}  // addtogroup math

} // namespace consteig

#endif

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

// Ordering matters: sqrt must precede complex.hpp (abs(Complex<T>) calls sqrt),
// and exp/trig must precede complex_exp.hpp (Complex exp calls exp/cos/sin).
#include "functions/abs.hpp"
#include "functions/sqrt.hpp"
#include "functions/pow.hpp"
#include "functions/sgn.hpp"
#include "functions/exp.hpp"
#include "functions/trig.hpp"

#endif

#include "complex.hpp"
#include "functions/complex_exp.hpp"
#include "functions/csqrt.hpp"
#include "functions/utilities.hpp"

#endif

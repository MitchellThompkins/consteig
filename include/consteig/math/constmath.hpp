#ifndef CONSTMATH_HPP
#define CONSTMATH_HPP

#include "../consteig_options.hpp"

// When gcem is enabled, include the gcem delegation layer first. It defines
// consteig::sqrt, abs, exp, sin, cos, tan, pow, sgn as thin wrappers around
// gcem::, and also includes complex.hpp. The individual function headers below
// detect CONSTEIG_USE_GCEM and skip their function bodies, becoming no-ops.
#ifdef CONSTEIG_USE_GCEM
#include "gcem_wrapper.hpp"
#endif

#include "complex.hpp"
#include "functions/abs.hpp"
#include "functions/csqrt.hpp"
#include "functions/exp.hpp"
#include "functions/pow.hpp"
#include "functions/sgn.hpp"
#include "functions/sqrt.hpp"
#include "functions/trig.hpp"
#include "functions/utilities.hpp"

#endif

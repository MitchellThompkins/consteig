#ifndef CONSTEIG_GCEM_WRAPPER_HPP
#define CONSTEIG_GCEM_WRAPPER_HPP

#include "../consteig_options.hpp"

// Configure gcem's type-traits mode BEFORE including any gcem header.
// Without CONSTEIG_GCEM_USE_STDLIB, gcem uses compiler builtins only
// (freestanding). With it, gcem uses <limits> and <type_traits>.
#ifndef CONSTEIG_GCEM_USE_STDLIB
#define GCEM_TRAITS_BUILTIN
#endif

#include "../optional_dependencies/gcem/gcem.hpp"

// consteig-specific utilities (epsilon, is_float, equalWithin, poison_nan).
// These are not provided by gcem and are always needed.
#include "functions/utilities.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

template <typename T>
constexpr T sqrt(const T x) noexcept
{
    return gcem::sqrt(x);
}

template <typename T>
constexpr T abs(const T x) noexcept
{
    return gcem::abs(x);
}

template <typename T>
constexpr auto exp(const T x) noexcept
{
    return gcem::exp(x);
}

template <typename T>
constexpr auto sin(const T x) noexcept
{
    return gcem::sin(x);
}

template <typename T>
constexpr auto cos(const T x) noexcept
{
    return gcem::cos(x);
}

template <typename T>
constexpr auto tan(const T x) noexcept
{
    return gcem::tan(x);
}

template <typename T>
constexpr T pow(const T x, const unsigned int n) noexcept
{
    return static_cast<T>(gcem::pow(x, n));
}

template <typename T>
constexpr T pow(const T x, const int n) noexcept
{
    return static_cast<T>(gcem::pow(x, n));
}

template <typename T>
constexpr T sgn(const T x) noexcept
{
    return gcem::sgn(x);
}

/// @}  // addtogroup math

} // namespace consteig

// Include Complex<T> type after the scalar wrappers are defined.
// abs(Complex<T>) calls consteig::sqrt which is now the gcem-backed version.
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
    T ex = consteig::exp(z.real);
    return {ex * consteig::cos(z.imag), ex * consteig::sin(z.imag)};
}

/// @}  // addtogroup math

} // namespace consteig

#endif

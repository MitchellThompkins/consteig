#ifndef CONSTMATH_TRIG_HPP
#define CONSTMATH_TRIG_HPP
// Reference: https://en.wikipedia.org/wiki/Taylor_series#Trigonometric_functions

#include "../../consteig_options.hpp"
#include "utilities.hpp"

namespace consteig
{

// Reduce x to (-pi, pi] by removing integer multiples of 2*pi
template <typename T>
constexpr T trig_reduce(const T x) noexcept
{
    constexpr T two_pi = static_cast<T>(2.0 * PI_CONST);
    T k = static_cast<T>(static_cast<long long>(x / two_pi));
    T r = x - k * two_pi;
    if (r > static_cast<T>(PI_CONST))
    {
        r -= two_pi;
    }
    else if (r <= -static_cast<T>(PI_CONST))
    {
        r += two_pi;
    }
    return r;
}

template <typename T>
constexpr T sin_series(const T x) noexcept
{
    // sin(x) = x - x^3/3! + x^5/5! - ...
    T result = x;
    T term = x;
    int n = 1;
    while (n <= CONSTEIG_TRIG_MAX_ITER)
    {
        term *= -x * x / static_cast<T>((2 * n) * (2 * n + 1));
        result += term;
        ++n;
    }
    return result;
}

template <typename T>
constexpr T cos_series(const T x) noexcept
{
    // cos(x) = 1 - x^2/2! + x^4/4! - ...
    T result = static_cast<T>(1);
    T term = static_cast<T>(1);
    int n = 1;
    while (n <= CONSTEIG_TRIG_MAX_ITER)
    {
        term *= -x * x / static_cast<T>((2 * n - 1) * (2 * n));
        result += term;
        ++n;
    }
    return result;
}

/**
 * @brief Computes the sine of x (in radians).
 */
template <typename T>
constexpr auto sin(const T x) noexcept
{
    if constexpr (!is_float<T>())
    {
        return sin_series(trig_reduce(static_cast<double>(x)));
    }
    else
    {
        return sin_series(trig_reduce(x));
    }
}

/**
 * @brief Computes the cosine of x (in radians).
 */
template <typename T>
constexpr auto cos(const T x) noexcept
{
    if constexpr (!is_float<T>())
    {
        return cos_series(trig_reduce(static_cast<double>(x)));
    }
    else
    {
        return cos_series(trig_reduce(x));
    }
}

/**
 * @brief Computes the tangent of x (in radians).
 */
template <typename T>
constexpr auto tan(const T x) noexcept
{
    if constexpr (!is_float<T>())
    {
        const double r = trig_reduce(static_cast<double>(x));
        return sin_series(r) / cos_series(r);
    }
    else
    {
        const T r = trig_reduce(x);
        return sin_series(r) / cos_series(r);
    }
}

} // namespace consteig

#endif

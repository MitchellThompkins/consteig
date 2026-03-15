#ifndef CONSTMATH_UTILITIES_HPP
#define CONSTMATH_UTILITIES_HPP

#include "abs.hpp"

namespace consteig
{

// These functions determine if a number is a floating point number

template <typename T> struct is_float_impl
{
    static constexpr bool _()
    {
        return false;
    }
};

template <> struct is_float_impl<float>
{
    static constexpr bool _()
    {
        return true;
    }
};

template <> struct is_float_impl<double>
{
    static constexpr bool _()
    {
        return true;
    }
};

template <> struct is_float_impl<long double>
{
    static constexpr bool _()
    {
        return true;
    }
};

template <typename T> constexpr bool is_float(T const &)
{
    return is_float_impl<T>::_();
}

// Check if float
template <typename T> constexpr bool is_float()
{
    return is_float_impl<T>::_();
}

// Compare Floats
template <typename T, typename U>
static constexpr bool approxEqual(T a, T b, U thresh)
{
    return consteig::abs(a - b) < static_cast<T>(thresh);
}

// Find machine epsilon
// Implemented from wikipedia
// https://en.wikipedia.org/wiki/Machine_epsilon#Approximation
template <typename T> constexpr T epsilon()
{
    if constexpr (!is_float<T>())
    {
        return static_cast<T>(0);
    }
    else
    {
        // Hardcoded for standard IEEE 754 to ensure O(1) performance in
        // recursion
        if (sizeof(T) == sizeof(float))
        {
            return static_cast<T>(1.19209290e-7);
        }
        if (sizeof(T) == sizeof(double))
        {
            return static_cast<T>(2.2204460492503131e-16);
        }

        // Fallback for long double or others: calculate iteratively
        T eps = static_cast<T>(1.0);
        T one = static_cast<T>(1.0);
        T half = static_cast<T>(0.5);

        while ((one + (half * eps)) != one)
        {
            eps = half * eps;
        }

        return eps;
    }
}

// This helper is intentionally NOT constexpr.
// If a user attempts to evaluate a negative square root at compile-time
// (e.g. `constexpr auto x = sqrt(-4);`), the compiler will hit this
// non-constexpr function and halt the build with an error mentioning this
// function's name. At runtime, it safely executes and returns a poison value.
// For floating-point types, we return 0.0 / 0.0 to produce a true
// self-poisoning NaN. For integer types, we use -1 as a poison value because
// integer division by zero crashes at runtime, and constexpr NaN is not
// portably supported in C++17.
template <typename T> T force_compile_time_error_for_negative_sqrt()
{
    if constexpr (is_float<T>())
    {
        return static_cast<T>(0.0) / static_cast<T>(0.0);
    }
    else
    {
        return static_cast<T>(-1);
    }
}

// Get a poison value representing an invalid result (like NaN).
template <typename T> constexpr T poison_nan()
{
    // We use a non-constexpr helper to guarantee that this function cannot be
    // evaluated at compile-time (triggering an error), but gracefully returns
    // the poison value at runtime.
    return force_compile_time_error_for_negative_sqrt<T>();
}

// Check if a value is the poison NaN.
template <typename T> constexpr bool is_poison_nan(const T x)
{
    if constexpr (is_float<T>())
    {
        // In IEEE 754, NaN is the only value not equal to itself.
        return x != x;
    }
    else
    {
        return x == static_cast<T>(-1);
    }
}

} // namespace consteig

#endif

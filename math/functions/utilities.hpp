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
static constexpr bool compareFloats(T a, T b, U thresh)
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

// Get a poison value representing an invalid result (like NaN).
template <typename T> constexpr T poison_nan()
{
    // We use -1.0 (or -1 for ints) as a poison value because constexpr NaN
    // is not portably supported without built-ins or stdlib dependencies in C++17.
    return static_cast<T>(-1);
}

// Check if a value is the poison NaN.
template <typename T> constexpr bool is_poison_nan(const T x)
{
    return x == static_cast<T>(-1);
}

} // namespace consteig

#endif

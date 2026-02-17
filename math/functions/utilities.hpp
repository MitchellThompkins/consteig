#ifndef CONSTMATH_UTILITIES_HPP
#define CONSTMATH_UTILITIES_HPP

#include "abs.hpp"

namespace consteig
{

// These functions determine if a number is a floating point number

template<typename T>
struct is_float_impl
{
    static constexpr bool _(){ return false; }
};

template<>
struct is_float_impl<float>
{
    static constexpr bool _() { return true; }
};

template<>
struct is_float_impl<double>
{
    static constexpr bool _() { return true; }
};

template<>
struct is_float_impl<long double>
{
    static constexpr bool _() { return true; }
};

template<typename T>
constexpr bool is_float(T const&)
{
    return is_float_impl<T>::_();
}

// Check if float
template<typename T>
constexpr bool is_float()
{
    return is_float_impl<T>::_();
}

// Compare Floats
template<typename T>
static constexpr bool compareFloats(T a, T b, T thresh)
{
    return consteig::abs(a - b) < thresh;
}

// Find machine epsilon
// Implemented from wikipedia
// https://en.wikipedia.org/wiki/Machine_epsilon#Approximation
template<typename T>
constexpr T epsilon()
{
    if (!is_float<T>()) {
        return static_cast<T>(0);
    }

    // Hardcoded for standard IEEE 754 to ensure O(1) performance in recursion
    if (sizeof(T) == sizeof(float)) {
        return static_cast<T>(1.19209290e-7);
    }
    if (sizeof(T) == sizeof(double)) {
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


} // end namespace

#endif

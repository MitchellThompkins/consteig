#ifndef CONSTMATH_UTILITIES_HPP
#define CONSTMATH_UTILITIES_HPP

#include "../constmath.hpp"

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
    T epsilon {static_cast<T>(0)};

    if(is_float<T>())
    {
        epsilon = 1.0;
        T one = 1.0;
        T half = 0.5;

        while ((one+half * epsilon) != one)
        {
            epsilon = half * epsilon;
        }
    }

    return epsilon;
}


} // end namespace

#endif

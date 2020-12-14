#ifndef MATRIX_TOOLS_HPP
#define MATRIX_TOOLS_HPP

#include "../math/constmath.hpp"

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

template<typename T>
constexpr bool is_float()
{
    return is_float_impl<T>::_();
}


template<typename T>
static constexpr bool compareFloats(T a, T b, T thresh)
{
    return consteig::abs(a - b) < thresh;
}

} // end namespace

#endif

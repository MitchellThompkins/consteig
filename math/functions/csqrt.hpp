#ifndef CONSTMATH_CSQRT_HPP
#define CONSTMATH_CSQRT_HPP

#include "../complex.hpp"
#include "abs.hpp"
#include "sqrt.hpp"
#include "utilities.hpp"

namespace consteig
{

template <typename T> constexpr Complex<T> csqrt(const T x)
{
    if (x < static_cast<T>(0))
    {
        if constexpr (is_float<T>())
        {
            return {static_cast<T>(0), consteig::sqrt(consteig::abs(x))};
        }
        else
        {
            // Cast to unsigned long long and negate to safely calculate the
            // absolute value of x, even if x is INT_MIN or INT64_MIN, avoiding
            // overflow.
            return {static_cast<T>(0),
                    static_cast<T>(
                        consteig::sqrt(-static_cast<unsigned long long>(x)))};
        }
    }
    else
    {
        return {consteig::sqrt(x), static_cast<T>(0)};
    }
}

} // namespace consteig

#endif
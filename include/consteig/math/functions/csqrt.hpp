#ifndef CONSTMATH_CSQRT_HPP
#define CONSTMATH_CSQRT_HPP

#include "../complex.hpp"
#include "utilities.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Complex-valued square root of a real number.
///
/// For non-negative `x`, returns `{sqrt(x), 0}`.
/// For negative `x`, returns `{0, sqrt(|x|)}` (purely imaginary result).
/// Use this instead of @ref sqrt when the input may be negative.
///
/// @tparam T  Numeric type.
/// @param  x  Input value (may be negative).
/// @return @ref Complex<T> square root.
template <typename T> constexpr Complex<T> csqrt(const T x)
{
    if (x < static_cast<T>(0))
    {
        if constexpr (is_float<T>())
        {
            return {static_cast<T>(0), sqrt(abs(x))};
        }
        else
        {
            // Cast to unsigned long long and negate to safely calculate the
            // absolute value of x, even if x is INT_MIN or INT64_MIN, avoiding
            // overflow.
            return {static_cast<T>(0),
                    static_cast<T>(
                        sqrt(-static_cast<unsigned long long>(x)))};
        }
    }
    else
    {
        return {sqrt(x), static_cast<T>(0)};
    }
}

/// @}  // addtogroup math

} // namespace consteig

#endif
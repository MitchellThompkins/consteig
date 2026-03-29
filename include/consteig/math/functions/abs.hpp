#ifndef CONSTMATH_ABS_HPP
#define CONSTMATH_ABS_HPP

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Absolute value of a real number.
///
/// Returns `|x|`. Handles signed zero correctly (returns positive zero).
///
/// @tparam T  Numeric type.
/// @param  x  Input value.
/// @return Non-negative absolute value of `x`.
template <typename T> constexpr T abs(const T x)
{
    return (
        // if 0 is signed
        x == T(0) ? T(0) :
                  // else
            x < T(0) ? -x
                     : x);
}

/// @}  // addtogroup math

} // namespace consteig

#endif

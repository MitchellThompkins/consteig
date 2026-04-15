#ifndef CONSTMATH_SGN_HPP
#define CONSTMATH_SGN_HPP

#include "../../consteig_options.hpp"

#ifndef CONSTEIG_USE_GCEM

namespace consteig
{

/// @addtogroup math
/// @{

/// @brief Signum function: returns +1, -1, or 0.
///
/// @tparam T  Numeric type.
/// @param  x  Input value.
/// @return `+1` if `x > 0`, `-1` if `x < 0`, `0` if `x == 0`.
template <typename T> constexpr T sgn(const T x)
{
    return ( // positive
        x > static_cast<T>(0) ? static_cast<T>(1) :
                              // negative
            x < static_cast<T>(0) ? static_cast<T>(-1)
                                  :
                                  // else
            static_cast<T>(0));
}

/// @}  // addtogroup math

} // namespace consteig

#endif // CONSTEIG_USE_GCEM

#endif

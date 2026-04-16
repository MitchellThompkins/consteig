#ifndef CONSTMATH_EXP_HPP
#define CONSTMATH_EXP_HPP

#include "../../consteig_options.hpp"
#include "pow.hpp"
#include "utilities.hpp"

namespace consteig
{

/// @addtogroup math
/// @{

// Max iterations for continued fraction expansion
constexpr int EXP_MAX_ITER = 50;

template <typename T>
constexpr T exp_cf_recur(const T x, const int depth_end) noexcept
{
    int depth = EXP_MAX_ITER - 1;
    T res = static_cast<T>(1);
    while (depth > depth_end - 1)
    {
        res = static_cast<T>(1) + x / static_cast<T>(depth - 1) -
              x / static_cast<T>(depth) / res;
        --depth;
    }
    return res;
}

template <typename T> constexpr T exp_cf(const T x) noexcept
{
    return static_cast<T>(1) / (static_cast<T>(1) - x / exp_cf_recur(x, 2));
}

template <typename T> constexpr T find_whole(const T x) noexcept
{
    return static_cast<T>(static_cast<long long>(x));
}

template <typename T> constexpr T find_fraction(const T x) noexcept
{
    return x - find_whole(x);
}

template <typename T> constexpr T exp_split(const T x) noexcept
{
    return pow(static_cast<T>(E_CONST), static_cast<int>(find_whole(x))) *
           exp_cf(find_fraction(x));
}

template <typename T> constexpr T exp_check(const T x) noexcept
{
    return (x < static_cast<T>(0)   ? static_cast<T>(1) / exp_check(-x)
            : x < static_cast<T>(2) ? exp_cf(x)
                                    : exp_split(x));
}

/**
 * @brief Computes the exponential of a real number.
 */
template <typename T> constexpr auto exp(const T x) noexcept
{
    if constexpr (!is_float<T>())
    {
        return exp_check(static_cast<double>(x));
    }
    else
    {
        return exp_check(x);
    }
}

/// @}  // addtogroup math

} // namespace consteig

#endif

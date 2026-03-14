#ifndef CONSTMATH_SQRT_HPP
#define CONSTMATH_SQRT_HPP

#include "../../consteig_options.hpp"
#include "utilities.hpp" // For epsilon

namespace consteig
{

template <typename T>
constexpr T sqrt_recur(const T x, const T xn, const int count)
{
    return (abs(xn - x / xn) / (T(1) + xn) < epsilon<T>() ? xn
            : count < CONSTEIG_MAX_ITER
                ? sqrt_recur(x, T(0.5) * (xn + x / xn), count + 1)
                : xn);
}

template <typename T> constexpr T sqrt_check(const T x, const T m_val)
{
    return (x == T(0)                      ? T(0)
            : epsilon<T>() > abs(T(1) - x) ? x
            : x > T(4)                     ? sqrt_check(x / T(4), T(2) * m_val)
            : x < T(0.25)                  ? sqrt_check(x * T(4), m_val / T(2))
                          : m_val * sqrt_recur(x, x / T(2), 0));
}

template <typename T> constexpr T sqrt_int(const T x)
{
    // The closed guess will be stored in the root
    T root{static_cast<T>(0)};

    // Base cases
    if (x == 0 || x == 1)
    {
        root = x;
    }
    else
    {
        // Starting from 1, try all numbers until
        // i is greater than x / i (avoids i*i overflow).
        T i = 1;
        while (i <= x / i)
        {
            i++;
        }
        root = i - 1;
    }

    return root;
}

template <typename T> constexpr T sqrt(const T x)
{
    // argument. Users should call csqrt(x) if x might be negative.
    if (x < static_cast<T>(0))
    {
        // We return a poison value (-1) as constexpr NaN is not portably
        // supported in C++17 without built-ins. In the future, this could
        // be replaced with a real NaN if a portable constexpr solution is
        // found.
        return poison_nan<T>();
    }

    if constexpr (is_float<T>())
        return sqrt_check(x, static_cast<T>(1));
    else
        return sqrt_int(x);
}

template <typename T> struct Complex;

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

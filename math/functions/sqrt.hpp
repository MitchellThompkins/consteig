#ifndef CONSTMATH_SQRT_HPP
#define CONSTMATH_SQRT_HPP

#include "utilities.hpp" // For epsilon
#include "../../consteig_options.hpp"

namespace consteig
{

namespace internal
{
template<typename T>
constexpr T sqrt_recur(
        const T x,
        const T xn,
        const int count)
{
    return( abs(xn - x/xn) / (T(1) + xn) < epsilon<T>() ? xn :
            count < CONSTEIG_SQRT_MAX_ITER ? \
            sqrt_recur(x,T(0.5)*(xn + x/xn), count+1) :
            xn );
}

template<typename T>
constexpr T sqrt_check(
        const T x,
        const T m_val)
{
    return( // indistinguishable from zero or one
            epsilon<T>() > abs(x) ? T(0) :
            epsilon<T>() > abs(T(1)-x) ? x :
            // else
            x > T(4) ? sqrt_check(x/T(4),T(2)*m_val) :
            m_val*sqrt_recur(x,x/T(2),0) );
}

template<typename T>
constexpr T sqrt_int(
        const T x )
{
    // The closed guess will be stored in the root
    T root {static_cast<T>(0)};

    // Base cases
    if (x == 0 || x == 1)
    {
        root = x;
    }
    else
    {
        // Staring from 1, try all numbers until
        // i*i is greater than or equal to val.
        T i = 1, result = 1;
        while (result <= x)
        {
            i++;
            result = i * i;
        }
        root = i - 1;
    }

    return root;
}

}

template<typename T>
constexpr T sqrt(const T x)
{
    // TODO(mthompkins): Need to return NaN for negative numbers. This
    // implementation is really really ugly
    if(x < static_cast<T>(0))
        return static_cast<T>(-1);
    else if(is_float<T>())
        return internal::sqrt_check( x, static_cast<T>(1) );
    else
        return internal::sqrt_int(x);
}

} //end namespace

#endif

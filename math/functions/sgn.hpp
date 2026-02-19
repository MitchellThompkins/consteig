#ifndef CONSTMATH_SGN_HPP
#define CONSTMATH_SGN_HPP

namespace consteig
{

template<typename T>
constexpr T sgn(
        const T x)
{
    return( // positive
            x > static_cast<T>(0) ?  static_cast<T>(1) :
            // negative
            x < static_cast<T>(0) ? static_cast<T>(-1) :
            // else
                static_cast<T>(0) );
}

} //end namespace

#endif

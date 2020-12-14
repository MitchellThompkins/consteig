#ifndef CONSTMATH_ABS_HPP
#define CONSTMATH_ABS_HPP

namespace consteig
{

template<typename T>
constexpr T abs(const T x)
{
    return(
            // if 0 is signed
            x == T(0) ? T(0) :
            // else
            x < T(0) ? -x : x );
}

} //end namespace

#endif

#ifndef CONSTMATH_POW_HPP
#define CONSTMATH_POW_HPP

namespace consteig
{

template<typename T>
constexpr T pow(const T x)
{
    return(
            // if 0 is signed
            x == T(0) ? T(0) :
            // else
            x < T(0) ? -x : x );
}

} //end namespace

#endif

#ifndef CONSTMATH_UTILITIES_HPP
#define CONSTMATH_UTILITIES_HPP

namespace consteig
{

template<typename T>
static constexpr bool compareFloats(T a, T b, T thresh)
{
    return consteig::abs(a - b) < thresh;
}

} // end namespace

#endif

#ifndef TEST_TOOLS_HPP
#define TEST_TOOLS_HPP

#include <limits>
#include <cfloat>

#include "../consteig.hpp"

#define MSG "Not constexpr or wrong output"

// https://stackoverflow.com/a/32334103/3527182
template<typename T>
constexpr bool nearlyEqual(
  T a,
  T b,
  T epsilon = 128 * FLT_EPSILON, T relth = FLT_MIN)
  // those defaults are arbitrary and could be removed
{
  static_assert(consteig::is_float<T>(), "Expects floating point number");
  assert(std::numeric_limits<T>::epsilon() <= epsilon);
  assert(epsilon < 1.f);

  if (a == b) return true;

  auto diff = std::abs(a-b);
  auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<T>::max());
  // or even faster: std::min(std::abs(a + b), std::numeric_limits<float>::max());
  // keeping this commented out until I update figures below
  return diff < std::max(relth, epsilon * norm);
}


template<size_t R, size_t C>
static constexpr bool compareFloatMat(
        consteig::Matrix<float,R,C> a,
        consteig::Matrix<float,R,C> b,
        const float thresh )
{
    for(int i {0}; i<R; i++)
    {
        for(int j {0}; j<C; j++)
        {
            if( !consteig::compareFloats(a(i,j), b(i,j), thresh) )
                return false;
        }
    }
    return true;
}

#endif

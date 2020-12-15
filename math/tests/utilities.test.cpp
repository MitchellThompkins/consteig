#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "constmath.hpp"

static constexpr double kThresh {0.0001};

using namespace consteig;

// https://stackoverflow.com/a/32334103/3527182
template<typename T>
constexpr bool nearlyEqual(
  T a, T b,
  T epsilon = 128 * FLT_EPSILON, T relth = FLT_MIN)
  // those defaults are arbitrary and could be removed
{
  static_assert(is_float<T>(), "Expects floating point number");
  assert(std::numeric_limits<T>::epsilon() <= epsilon);
  assert(epsilon < 1.f);

  if (a == b) return true;

  auto diff = std::abs(a-b);
  auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<T>::max());
  // or even faster: std::min(std::abs(a + b), std::numeric_limits<float>::max());
  // keeping this commented out until I update figures below
  return diff < std::max(relth, epsilon * norm);
}

TEST(utilities, compare_floats)
{
    static constexpr double aCompare1 {45564813e-4};
    static constexpr double aCompare2 {45564813e-4};
    static constexpr bool aCompareTest = compareFloats(aCompare1, aCompare2, kThresh);
    static constexpr bool aCompareAns  = nearlyEqual(aCompare1, aCompare2);
    static_assert(aCompareTest==aCompareAns, MSG);
    ASSERT_TRUE(aCompareTest==aCompareAns);

    static constexpr double bCompare1 {0.0000};
    static constexpr double bCompare2 {0.0000};
    static constexpr bool bCompareTest = compareFloats(bCompare1, bCompare2, kThresh);
    static constexpr bool bCompareAns  = nearlyEqual(bCompare1, bCompare2);
    static_assert(bCompareTest==bCompareAns, MSG);
    ASSERT_TRUE(bCompareTest==bCompareAns);
}

TEST(utilities, compare_floats_compare)
{
    static constexpr double compare1 {1834.2422435};
    static constexpr double compare2 {1834.2422436};

    static constexpr bool compareTest1 = compareFloats(compare1, compare2, 0.0000001);
    static constexpr bool compareTest2 = compareFloats(compare1, compare2, 0.00000001);

    static_assert(compareTest1==true, MSG);
    static_assert(compareTest2==false, MSG);
    ASSERT_TRUE(compareTest1==true);
    ASSERT_TRUE(compareTest2==false);
}

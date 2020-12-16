#include <gtest/gtest.h>
#include <limits>
#include "test_tools.hpp"

#include "constmath.hpp"

static constexpr double kThresh {0.00000001};

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

TEST(utilities, value_type_check)
{
    static constexpr float x {9.4};
    static constexpr double y {45.6};
    static constexpr long double z {-12.3};

    static constexpr size_t a {9};
    static constexpr int b {45};
    static constexpr long int c {-12};
    static constexpr long long int d {-12};

    static_assert(is_float(x), MSG);
    static_assert(is_float(y), MSG);
    static_assert(is_float(z), MSG);

    static_assert(!is_float(a), MSG);
    static_assert(!is_float(b), MSG);
    static_assert(!is_float(c), MSG);
    static_assert(!is_float(d), MSG);

    ASSERT_TRUE(is_float(x));
    ASSERT_TRUE(is_float(y));
    ASSERT_TRUE(is_float(z));

    ASSERT_FALSE(is_float(a));
    ASSERT_FALSE(is_float(b));
    ASSERT_FALSE(is_float(c));
    ASSERT_FALSE(is_float(d));
}

TEST(utilities, template_type_check)
{
    static_assert(is_float<float>(), MSG);
    static_assert(is_float<double>(), MSG);
    static_assert(is_float<long double>(), MSG);

    static_assert(!is_float<size_t>(), MSG);
    static_assert(!is_float<int>(), MSG);
    static_assert(!is_float<long int>(), MSG);
    static_assert(!is_float<long long int>(), MSG);

    ASSERT_TRUE(is_float<float>());
    ASSERT_TRUE(is_float<double>());
    ASSERT_TRUE(is_float<long double>());

    ASSERT_FALSE(is_float<size_t>());
    ASSERT_FALSE(is_float<int>());
    ASSERT_FALSE(is_float<long int>());
    ASSERT_FALSE(is_float<long long int>());
}

TEST(utilities, machine_epsilon)
{
    static_assert(epsilon<float>()==std::numeric_limits<float>::epsilon(), MSG);
    static_assert(epsilon<double>()==std::numeric_limits<double>::epsilon(), MSG);
    static_assert(epsilon<long double>()==std::numeric_limits<long double>::epsilon(), MSG);
    static_assert(epsilon<int>()==std::numeric_limits<int>::epsilon(), MSG);
    static_assert(epsilon<unsigned int>()==std::numeric_limits<unsigned int>::epsilon(), MSG);
    static_assert(epsilon<char>()==std::numeric_limits<char>::epsilon(), MSG);
    static_assert(epsilon<bool>()==std::numeric_limits<bool>::epsilon(), MSG);

    ASSERT_TRUE(epsilon<float>()==std::numeric_limits<float>::epsilon());
    ASSERT_TRUE(epsilon<double>()==std::numeric_limits<double>::epsilon());
    ASSERT_TRUE(epsilon<long double>()==std::numeric_limits<long double>::epsilon());
    ASSERT_TRUE(epsilon<int>()==std::numeric_limits<int>::epsilon());
    ASSERT_TRUE(epsilon<unsigned int>()==std::numeric_limits<unsigned int>::epsilon());
    ASSERT_TRUE(epsilon<char>()==std::numeric_limits<char>::epsilon());
    ASSERT_TRUE(epsilon<bool>()==std::numeric_limits<bool>::epsilon());
}

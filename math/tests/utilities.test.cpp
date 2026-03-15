#include <gtest/gtest.h>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(utilities, compare_floats)
{
    static constexpr double aCompare1{45564813e-4};
    static constexpr double aCompare2{45564813e-4};
    static constexpr bool aCompareTest =
        compareFloats(aCompare1, aCompare2, CONSTEIG_TEST_TOLERANCE);
    static constexpr bool aCompareAns = nearlyEqual(aCompare1, aCompare2);
    static_assert(aCompareTest == aCompareAns, MSG);
    ASSERT_TRUE(aCompareTest == aCompareAns);

    static constexpr double bCompare1{0.0000};
    static constexpr double bCompare2{0.0000};
    static constexpr bool bCompareTest =
        compareFloats(bCompare1, bCompare2, CONSTEIG_TEST_TOLERANCE);
    static constexpr bool bCompareAns = nearlyEqual(bCompare1, bCompare2);
    static_assert(bCompareTest == bCompareAns, MSG);
    ASSERT_TRUE(bCompareTest == bCompareAns);
}

TEST(utilities, compare_floats_compare)
{
    static constexpr double compare1{1834.2422435};
    static constexpr double compare2{1834.2422436};

    static constexpr bool compareTest1 =
        compareFloats(compare1, compare2, 1e-6); // Design-intent: Expect true
    static constexpr bool compareTest2 =
        compareFloats(compare1, compare2, 1e-8); // Design-intent: Expect false

    static_assert(compareTest1 == true, MSG);
    static_assert(compareTest2 == false, MSG);
    ASSERT_TRUE(compareTest1 == true);
    ASSERT_TRUE(compareTest2 == false);
}

TEST(utilities, value_type_check)
{
    static constexpr float x{9.4f};
    static constexpr double y{45.6};
    static constexpr long double z{-12.3L};

    static constexpr Size a{9};
    static constexpr int b{45};
    static constexpr long int c{-12};
    static constexpr long long int d{-12};

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

    static_assert(!is_float<Size>(), MSG);
    static_assert(!is_float<int>(), MSG);
    static_assert(!is_float<long int>(), MSG);
    static_assert(!is_float<long long int>(), MSG);

    ASSERT_TRUE(is_float<float>());
    ASSERT_TRUE(is_float<double>());
    ASSERT_TRUE(is_float<long double>());

    ASSERT_FALSE(is_float<Size>());
    ASSERT_FALSE(is_float<int>());
    ASSERT_FALSE(is_float<long int>());
    ASSERT_FALSE(is_float<long long int>());
}

TEST(utilities, machine_epsilon)
{
    static_assert(epsilon<float>() == std::numeric_limits<float>::epsilon(),
                  MSG);
    static_assert(epsilon<double>() == std::numeric_limits<double>::epsilon(),
                  MSG);
    static_assert(epsilon<long double>() ==
                      std::numeric_limits<long double>::epsilon(),
                  MSG);
    static_assert(epsilon<int>() == std::numeric_limits<int>::epsilon(), MSG);
    static_assert(epsilon<unsigned int>() ==
                      std::numeric_limits<unsigned int>::epsilon(),
                  MSG);
    static_assert(epsilon<char>() == std::numeric_limits<char>::epsilon(), MSG);
    static_assert(epsilon<bool>() == std::numeric_limits<bool>::epsilon(), MSG);

    ASSERT_TRUE(epsilon<float>() == std::numeric_limits<float>::epsilon());
    ASSERT_TRUE(epsilon<double>() == std::numeric_limits<double>::epsilon());
    ASSERT_TRUE(epsilon<long double>() ==
                std::numeric_limits<long double>::epsilon());
    ASSERT_TRUE(epsilon<int>() == std::numeric_limits<int>::epsilon());
    ASSERT_TRUE(epsilon<unsigned int>() ==
                std::numeric_limits<unsigned int>::epsilon());
    ASSERT_TRUE(epsilon<char>() == std::numeric_limits<char>::epsilon());
    ASSERT_TRUE(epsilon<bool>() == std::numeric_limits<bool>::epsilon());
}

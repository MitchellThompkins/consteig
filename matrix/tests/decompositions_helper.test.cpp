#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "decompositions.hpp"

using namespace consteig;

TEST(decompositions_helper, value_type_check)
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

TEST(decompositions_helper, template_type_check)
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

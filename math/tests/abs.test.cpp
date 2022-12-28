#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "constmath.hpp"

static constexpr double kThresh {0.00000001};

using namespace consteig;

TEST(absolute_value, abs)
{
    static_assert(consteig::compareFloats(consteig::abs(0.0), 0.0, kThresh), MSG);

    ASSERT_FLOAT_EQ(consteig::abs(0.0), 0.0);
    ASSERT_FLOAT_EQ(consteig::abs(-0.0),-0.0);

    ASSERT_FLOAT_EQ(consteig::abs(1.0), 1.0);
    ASSERT_FLOAT_EQ(consteig::abs(-1.0),1.0);

    ASSERT_FLOAT_EQ(consteig::abs(0U), 0U);
    ASSERT_FLOAT_EQ(consteig::abs(0), 0);

    ASSERT_FLOAT_EQ(consteig::abs(1U), 1U);
    ASSERT_FLOAT_EQ(consteig::abs(1), 1);
    ASSERT_FLOAT_EQ(consteig::abs(-1),1);
}

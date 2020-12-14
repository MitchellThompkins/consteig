#include <gtest/gtest.h>

#include "constmath.hpp"

TEST(absolute_value, abs)
{
    ASSERT_FLOAT_EQ(consteig::abs(0.0), 0.0);
    ASSERT_FLOAT_EQ(consteig::abs(-0.0),-0.0);

    ASSERT_FLOAT_EQ(consteig::abs(1.0), 1.0);
    ASSERT_FLOAT_EQ(consteig::abs(-1.0),-1.0);

    ASSERT_FLOAT_EQ(consteig::abs(0U), 0U);
    ASSERT_FLOAT_EQ(consteig::abs(0), 0);

    ASSERT_FLOAT_EQ(consteig::abs(1U), 1U);
    ASSERT_FLOAT_EQ(consteig::abs(1), 1);
    ASSERT_FLOAT_EQ(consteig::abs(-1),-1);
}

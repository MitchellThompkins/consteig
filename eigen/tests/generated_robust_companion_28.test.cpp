#include "generated_test_helpers.hpp"
TEST(generated_tests, companion_8x8_28)
{
    static_assert(check_single_companion_nonsym_8x8<28>(), "Test companion_8x8_28 failed");
    SUCCEED();
}

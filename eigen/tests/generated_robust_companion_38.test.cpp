#include "generated_test_helpers.hpp"
TEST(generated_tests, companion_8x8_38)
{
    static_assert(check_single_companion_nonsym_8x8<38>(), "Test companion_8x8_38 failed");
    SUCCEED();
}

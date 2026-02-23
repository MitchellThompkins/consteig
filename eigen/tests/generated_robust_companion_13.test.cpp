#include "generated_test_helpers.hpp"
TEST(generated_tests, companion_8x8_13)
{
    static_assert(check_single_companion_nonsym_8x8<13>(),
                  "Test companion_8x8_13 failed");
    SUCCEED();
}

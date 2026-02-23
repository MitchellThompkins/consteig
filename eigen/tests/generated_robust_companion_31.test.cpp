#include "generated_test_helpers.hpp"
TEST(generated_tests, companion_8x8_31)
{
    static_assert(check_single_companion_nonsym_8x8<31>(),
                  "Test companion_8x8_31 failed");
    SUCCEED();
}

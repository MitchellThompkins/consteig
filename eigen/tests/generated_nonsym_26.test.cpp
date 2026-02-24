#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_26)
{
    static_assert(check_single_random_nonsym_8x8<26>(),
                  "Test random_8x8_26 failed");
    SUCCEED();
}

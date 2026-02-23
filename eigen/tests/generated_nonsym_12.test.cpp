#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_12)
{
    static_assert(check_single_random_nonsym_8x8<12>(),
                  "Test random_8x8_12 failed");
    SUCCEED();
}

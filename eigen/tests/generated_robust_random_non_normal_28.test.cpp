#include "generated_test_helpers.hpp"
TEST(generated_tests, random_non_normal_8x8_28)
{
    static_assert(check_single_random_non_normal_nonsym_8x8<28>(),
                  "Test random_non_normal_8x8_28 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, non_normal_8x8_10)
{
    static_assert(check_single_non_normal_nonsym_8x8<10>(),
                  "Test non_normal_8x8_10 failed");
    SUCCEED();
}

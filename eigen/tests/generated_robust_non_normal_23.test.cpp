#include "generated_test_helpers.hpp"
TEST(generated_tests, non_normal_8x8_23)
{
    static_assert(check_single_non_normal_nonsym_8x8<23>(),
                  "Test non_normal_8x8_23 failed");
    SUCCEED();
}

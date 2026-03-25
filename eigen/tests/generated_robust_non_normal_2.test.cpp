#include "generated_test_helpers.hpp"
TEST(generated_tests, non_normal_8x8_2)
{
    static_assert(check_single_non_normal_nonsym_8x8<2>(), "Test non_normal_8x8_2 failed");
    SUCCEED();
}

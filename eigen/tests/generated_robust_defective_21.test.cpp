#include "generated_test_helpers.hpp"
TEST(generated_tests, defective_8x8_21)
{
    static_assert(check_single_defective_nonsym_8x8<21>(), "Test defective_8x8_21 failed");
    SUCCEED();
}

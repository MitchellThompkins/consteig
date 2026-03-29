#include "generated_test_helpers.hpp"
TEST(generated_tests, large_jordan_8x8_38)
{
    static_assert(check_single_large_jordan_nonsym_8x8<38>(), "Test large_jordan_8x8_38 failed");
    SUCCEED();
}

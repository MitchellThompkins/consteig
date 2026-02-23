#include "generated_test_helpers.hpp"
TEST(generated_tests, toeplitz_8x8_26)
{
    static_assert(check_single_toeplitz_nonsym_8x8<26>(),
                  "Test toeplitz_8x8_26 failed");
    SUCCEED();
}

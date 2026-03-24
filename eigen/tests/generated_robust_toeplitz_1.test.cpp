#include "generated_test_helpers.hpp"
TEST(generated_tests, toeplitz_8x8_1)
{
    static_assert(check_single_toeplitz_nonsym_8x8<1>(),
                  "Test toeplitz_8x8_1 failed");
    SUCCEED();
}

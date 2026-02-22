#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, toeplitz_8x8_49)
{
    static_assert(check_single_toeplitz_nonsym_8x8<49>(),
                  "Test toeplitz_8x8_49 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, toeplitz_10x10_1)
{
    static_assert(check_single_toeplitz_nonsym_10x10<1>(),
                  "Test toeplitz_10x10_1 failed");
    SUCCEED();
}
#endif

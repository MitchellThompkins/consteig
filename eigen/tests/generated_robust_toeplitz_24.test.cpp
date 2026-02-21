#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, toeplitz_8x8_24) {
    static_assert(check_single_toeplitz_nonsym_8x8<24>(),
                  "Test toeplitz_8x8_24 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, large_jordan_8x8_15) {
    static_assert(check_single_large_jordan_nonsym_8x8<15>(),
                  "Test large_jordan_8x8_15 failed");
    SUCCEED();
}
#endif

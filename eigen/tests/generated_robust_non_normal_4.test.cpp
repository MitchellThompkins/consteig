#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, non_normal_8x8_4) {
    static_assert(check_single_non_normal_nonsym_8x8<4>(), "Test non_normal_8x8_4 failed");
    SUCCEED();
}
#endif

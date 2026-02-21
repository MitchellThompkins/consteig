#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_8x8_44) {
    static_assert(check_single_graded_nonsym_8x8<44>(), "Test graded_8x8_44 failed");
    SUCCEED();
}
#endif

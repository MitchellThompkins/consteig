#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, companion_8x8_2) {
    static_assert(check_single_companion_nonsym_8x8<2>(), "Test companion_8x8_2 failed");
    SUCCEED();
}
#endif

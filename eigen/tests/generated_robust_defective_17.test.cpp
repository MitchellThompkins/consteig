#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, defective_8x8_17) {
    static_assert(check_single_defective_nonsym_8x8<17>(), "Test defective_8x8_17 failed");
    SUCCEED();
}
#endif

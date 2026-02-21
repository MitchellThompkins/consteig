#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, repeated_8x8_3) {
    static_assert(check_single_repeated_nonsym_8x8<3>(), "Test repeated_8x8_3 failed");
    SUCCEED();
}
#endif

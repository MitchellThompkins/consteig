#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_slow_0) {
    EXPECT_TRUE(check_single_companion_nonsym_slow<0>()) << "Test companion_slow_0 failed";
}
#endif
#endif

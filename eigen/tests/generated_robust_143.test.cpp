#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, non_normal_slow_1) {
    EXPECT_TRUE(check_single_non_normal_nonsym_slow<1>()) << "Test non_normal_slow_1 failed";
}
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, non_normal_fast_7) {
    EXPECT_TRUE(check_single_non_normal_nonsym_fast<7>()) << "Test non_normal_fast_7 failed";
}
#endif
#endif

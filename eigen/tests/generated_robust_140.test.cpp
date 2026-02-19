#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, non_normal_fast_0) { EXPECT_TRUE(check_single_non_normal_nonsym_fast<0>()) << "Test non_normal_fast_0 failed"; }
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_fast_4) { EXPECT_TRUE(check_single_random_non_normal_nonsym_fast<4>()) << "Test random_non_normal_fast_4 failed"; }
#endif
#endif

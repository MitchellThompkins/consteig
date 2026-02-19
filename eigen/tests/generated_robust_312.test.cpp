#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_fast_6) { EXPECT_TRUE(check_single_random_non_normal_nonsym_fast<6>()) << "Test random_non_normal_fast_6 failed"; }
#endif
#endif

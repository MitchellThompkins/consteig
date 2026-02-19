#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, toeplitz_fast_7) { EXPECT_TRUE(check_single_toeplitz_nonsym_fast<7>()) << "Test toeplitz_fast_7 failed"; }
#endif
#endif

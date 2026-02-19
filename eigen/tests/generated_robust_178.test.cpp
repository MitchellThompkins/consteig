#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_fast_9) { EXPECT_TRUE(check_single_clustered_nonsym_fast<9>()) << "Test clustered_fast_9 failed"; }
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_fast_2) { EXPECT_TRUE(check_single_clustered_nonsym_fast<2>()) << "Test clustered_fast_2 failed"; }
#endif
#endif

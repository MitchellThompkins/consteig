#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_fast_4) { EXPECT_TRUE(check_single_clustered_nonsym_fast<4>()) << "Test clustered_fast_4 failed"; }
#endif
#endif

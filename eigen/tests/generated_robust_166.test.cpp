#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_fast_3) { EXPECT_TRUE(check_single_clustered_nonsym_fast<3>()) << "Test clustered_fast_3 failed"; }
#endif
#endif

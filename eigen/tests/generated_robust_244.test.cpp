#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_fast_2) { EXPECT_TRUE(check_single_large_jordan_nonsym_fast<2>()) << "Test large_jordan_fast_2 failed"; }
#endif
#endif

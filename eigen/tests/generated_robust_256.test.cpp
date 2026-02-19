#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_fast_8) { EXPECT_TRUE(check_single_large_jordan_nonsym_fast<8>()) << "Test large_jordan_fast_8 failed"; }
#endif
#endif

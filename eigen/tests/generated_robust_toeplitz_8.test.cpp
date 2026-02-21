#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, toeplitz_fast_8) { static_assert(check_single_toeplitz_nonsym_fast<8>(), "Test toeplitz_fast_8 failed"); SUCCEED(); }
#else
TEST(generated_tests, toeplitz_slow_8) { static_assert(check_single_toeplitz_nonsym_slow<8>(), "Test toeplitz_slow_8 failed"); SUCCEED(); }
#endif
#endif

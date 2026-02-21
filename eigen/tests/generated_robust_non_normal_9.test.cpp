#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, non_normal_fast_9) { static_assert(check_single_non_normal_nonsym_fast<9>(), "Test non_normal_fast_9 failed"); SUCCEED(); }
#else
TEST(generated_tests, non_normal_slow_9) { static_assert(check_single_non_normal_nonsym_slow<9>(), "Test non_normal_slow_9 failed"); SUCCEED(); }
#endif
#endif

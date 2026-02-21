#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_fast_0) { static_assert(check_single_random_non_normal_nonsym_fast<0>(), "Test random_non_normal_fast_0 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_non_normal_slow_0) { static_assert(check_single_random_non_normal_nonsym_slow<0>(), "Test random_non_normal_slow_0 failed"); SUCCEED(); }
#endif
#endif

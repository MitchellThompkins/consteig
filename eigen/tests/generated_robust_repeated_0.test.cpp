#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_fast_0) { static_assert(check_single_repeated_nonsym_fast<0>(), "Test repeated_fast_0 failed"); SUCCEED(); }
#else
TEST(generated_tests, repeated_slow_0) { static_assert(check_single_repeated_nonsym_slow<0>(), "Test repeated_slow_0 failed"); SUCCEED(); }
#endif
#endif

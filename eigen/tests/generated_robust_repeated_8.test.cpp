#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_fast_8) { static_assert(check_single_repeated_nonsym_fast<8>(), "Test repeated_fast_8 failed"); SUCCEED(); }
#else
TEST(generated_tests, repeated_slow_8) { static_assert(check_single_repeated_nonsym_slow<8>(), "Test repeated_slow_8 failed"); SUCCEED(); }
#endif
#endif

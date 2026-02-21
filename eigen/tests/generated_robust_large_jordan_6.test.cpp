#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_fast_6) { static_assert(check_single_large_jordan_nonsym_fast<6>(), "Test large_jordan_fast_6 failed"); SUCCEED(); }
#else
TEST(generated_tests, large_jordan_slow_6) { static_assert(check_single_large_jordan_nonsym_slow<6>(), "Test large_jordan_slow_6 failed"); SUCCEED(); }
#endif
#endif

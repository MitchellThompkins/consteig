#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_9) { static_assert(check_single_random_nonsym_fast<9>(), "Test random_fast_9 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_9) { static_assert(check_single_random_nonsym_slow<9>(), "Test random_slow_9 failed"); SUCCEED(); }
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_32) { static_assert(check_single_random_nonsym_fast<32>(), "Test random_fast_32 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_32) { static_assert(check_single_random_nonsym_slow<32>(), "Test random_slow_32 failed"); SUCCEED(); }
#endif

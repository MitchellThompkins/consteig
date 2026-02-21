#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_5) { static_assert(check_single_random_nonsym_fast<5>(), "Test random_fast_5 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_5) { static_assert(check_single_random_nonsym_slow<5>(), "Test random_slow_5 failed"); SUCCEED(); }
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_1) { static_assert(check_single_random_nonsym_fast<1>(), "Test random_fast_1 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_1) { static_assert(check_single_random_nonsym_slow<1>(), "Test random_slow_1 failed"); SUCCEED(); }
#endif

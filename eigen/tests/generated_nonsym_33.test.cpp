#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_33) { static_assert(check_single_random_nonsym_fast<33>(), "Test random_fast_33 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_33) { static_assert(check_single_random_nonsym_slow<33>(), "Test random_slow_33 failed"); SUCCEED(); }
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_40) { static_assert(check_single_random_nonsym_fast<40>(), "Test random_fast_40 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_40) { static_assert(check_single_random_nonsym_slow<40>(), "Test random_slow_40 failed"); SUCCEED(); }
#endif

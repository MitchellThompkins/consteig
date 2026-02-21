#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_42) { static_assert(check_single_random_nonsym_fast<42>(), "Test random_fast_42 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_42) { static_assert(check_single_random_nonsym_slow<42>(), "Test random_slow_42 failed"); SUCCEED(); }
#endif

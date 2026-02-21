#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_35) { static_assert(check_single_random_sym_fast<35>(), "Test random_fast_35 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_35) { static_assert(check_single_random_sym_slow<35>(), "Test random_slow_35 failed"); SUCCEED(); }
#endif

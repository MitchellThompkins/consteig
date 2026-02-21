#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_36) { static_assert(check_single_random_sym_fast<36>(), "Test random_fast_36 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_36) { static_assert(check_single_random_sym_slow<36>(), "Test random_slow_36 failed"); SUCCEED(); }
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_4) { static_assert(check_single_random_sym_fast<4>(), "Test random_fast_4 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_4) { static_assert(check_single_random_sym_slow<4>(), "Test random_slow_4 failed"); SUCCEED(); }
#endif

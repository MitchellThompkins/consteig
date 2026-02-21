#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_8) { static_assert(check_single_random_sym_fast<8>(), "Test random_fast_8 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_8) { static_assert(check_single_random_sym_slow<8>(), "Test random_slow_8 failed"); SUCCEED(); }
#endif

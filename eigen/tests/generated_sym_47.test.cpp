#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_47) { static_assert(check_single_random_sym_fast<47>(), "Test random_fast_47 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_47) { static_assert(check_single_random_sym_slow<47>(), "Test random_slow_47 failed"); SUCCEED(); }
#endif

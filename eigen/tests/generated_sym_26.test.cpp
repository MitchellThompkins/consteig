#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_26) { static_assert(check_single_random_sym_fast<26>(), "Test random_fast_26 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_26) { static_assert(check_single_random_sym_slow<26>(), "Test random_slow_26 failed"); SUCCEED(); }
#endif

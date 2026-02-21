#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_37) { static_assert(check_single_random_sym_fast<37>(), "Test random_fast_37 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_37) { static_assert(check_single_random_sym_slow<37>(), "Test random_slow_37 failed"); SUCCEED(); }
#endif

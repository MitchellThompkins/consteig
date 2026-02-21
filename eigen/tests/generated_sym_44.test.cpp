#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_44) { static_assert(check_single_random_sym_fast<44>(), "Test random_fast_44 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_44) { static_assert(check_single_random_sym_slow<44>(), "Test random_slow_44 failed"); SUCCEED(); }
#endif

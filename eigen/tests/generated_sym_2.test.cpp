#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_2) { static_assert(check_single_random_sym_fast<2>(), "Test random_fast_2 failed"); SUCCEED(); }
#endif

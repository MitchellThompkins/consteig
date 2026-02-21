#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_43) { static_assert(check_single_random_sym_fast<43>(), "Test random_fast_43 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_43) { static_assert(check_single_random_sym_slow<43>(), "Test random_slow_43 failed"); SUCCEED(); }
#endif

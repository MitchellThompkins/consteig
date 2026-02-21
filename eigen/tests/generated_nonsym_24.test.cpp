#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_24) { static_assert(check_single_random_nonsym_fast<24>(), "Test random_fast_24 failed"); SUCCEED(); }
#else
TEST(generated_tests, random_slow_24) { static_assert(check_single_random_nonsym_slow<24>(), "Test random_slow_24 failed"); SUCCEED(); }
#endif

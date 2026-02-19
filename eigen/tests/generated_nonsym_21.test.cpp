#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_21) { static_assert(check_single_nonsymmetric_fast<21>(), "NonSym Fast 21 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_21) { static_assert(check_single_nonsymmetric_slow<21>(), "NonSym Slow 21 failed"); SUCCEED(); }
#endif

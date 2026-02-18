#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_2) { static_assert(check_single_nonsymmetric_fast<2>(), "NonSym Fast 2 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_2) { static_assert(check_single_nonsymmetric_slow<2>(), "NonSym Slow 2 failed"); SUCCEED(); }
#endif

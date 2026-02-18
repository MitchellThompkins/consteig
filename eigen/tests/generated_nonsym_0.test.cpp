#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_0) { static_assert(check_single_nonsymmetric_fast<0>(), "NonSym Fast 0 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_0) { static_assert(check_single_nonsymmetric_slow<0>(), "NonSym Slow 0 failed"); SUCCEED(); }
#endif

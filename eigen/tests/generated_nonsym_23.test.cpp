#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_23) { static_assert(check_single_nonsymmetric_fast<23>(), "NonSym Fast 23 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_23) { static_assert(check_single_nonsymmetric_slow<23>(), "NonSym Slow 23 failed"); SUCCEED(); }
#endif

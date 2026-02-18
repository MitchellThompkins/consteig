#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_13) { static_assert(check_single_nonsymmetric_fast<13>(), "NonSym Fast 13 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_13) { static_assert(check_single_nonsymmetric_slow<13>(), "NonSym Slow 13 failed"); SUCCEED(); }
#endif

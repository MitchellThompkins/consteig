#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_49) { static_assert(check_single_nonsymmetric_fast<49>(), "NonSym Fast 49 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_49) { static_assert(check_single_nonsymmetric_slow<49>(), "NonSym Slow 49 failed"); SUCCEED(); }
#endif

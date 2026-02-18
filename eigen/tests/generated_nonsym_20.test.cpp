#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_20) { static_assert(check_single_nonsymmetric_fast<20>(), "NonSym Fast 20 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_20) { static_assert(check_single_nonsymmetric_slow<20>(), "NonSym Slow 20 failed"); SUCCEED(); }
#endif

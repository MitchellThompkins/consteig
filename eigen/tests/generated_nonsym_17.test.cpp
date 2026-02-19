#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_17) { static_assert(check_single_nonsymmetric_fast<17>(), "NonSym Fast 17 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_17) { static_assert(check_single_nonsymmetric_slow<17>(), "NonSym Slow 17 failed"); SUCCEED(); }
#endif

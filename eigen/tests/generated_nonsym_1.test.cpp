#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_1) { static_assert(check_single_nonsymmetric_fast<1>(), "NonSym Fast 1 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_1) { static_assert(check_single_nonsymmetric_slow<1>(), "NonSym Slow 1 failed"); SUCCEED(); }
#endif

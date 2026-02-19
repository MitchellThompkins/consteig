#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_36) { static_assert(check_single_nonsymmetric_fast<36>(), "NonSym Fast 36 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_36) { static_assert(check_single_nonsymmetric_slow<36>(), "NonSym Slow 36 failed"); SUCCEED(); }
#endif

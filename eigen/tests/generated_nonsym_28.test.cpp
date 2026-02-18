#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_28) { static_assert(check_single_nonsymmetric_fast<28>(), "NonSym Fast 28 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_28) { static_assert(check_single_nonsymmetric_slow<28>(), "NonSym Slow 28 failed"); SUCCEED(); }
#endif

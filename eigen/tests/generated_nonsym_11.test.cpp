#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_11) { static_assert(check_single_nonsymmetric_fast<11>(), "NonSym Fast 11 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_11) { static_assert(check_single_nonsymmetric_slow<11>(), "NonSym Slow 11 failed"); SUCCEED(); }
#endif

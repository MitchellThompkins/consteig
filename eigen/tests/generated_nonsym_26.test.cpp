#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_26) { static_assert(check_single_nonsymmetric_fast<26>(), "NonSym Fast 26 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_26) { static_assert(check_single_nonsymmetric_slow<26>(), "NonSym Slow 26 failed"); SUCCEED(); }
#endif

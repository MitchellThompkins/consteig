#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_10) { static_assert(check_single_nonsymmetric_fast<10>(), "NonSym Fast 10 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_10) { static_assert(check_single_nonsymmetric_slow<10>(), "NonSym Slow 10 failed"); SUCCEED(); }
#endif

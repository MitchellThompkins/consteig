#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_4) { static_assert(check_single_nonsymmetric_fast<4>(), "NonSym Fast 4 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_4) { static_assert(check_single_nonsymmetric_slow<4>(), "NonSym Slow 4 failed"); SUCCEED(); }
#endif

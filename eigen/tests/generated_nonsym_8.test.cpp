#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_8) { static_assert(check_single_nonsymmetric_fast<8>(), "NonSym Fast 8 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_8) { static_assert(check_single_nonsymmetric_slow<8>(), "NonSym Slow 8 failed"); SUCCEED(); }
#endif

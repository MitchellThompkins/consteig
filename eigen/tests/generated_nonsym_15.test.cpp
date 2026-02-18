#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_15) { static_assert(check_single_nonsymmetric_fast<15>(), "NonSym Fast 15 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_15) { static_assert(check_single_nonsymmetric_slow<15>(), "NonSym Slow 15 failed"); SUCCEED(); }
#endif

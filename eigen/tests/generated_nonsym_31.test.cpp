#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_31) { static_assert(check_single_nonsymmetric_fast<31>(), "NonSym Fast 31 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_31) { static_assert(check_single_nonsymmetric_slow<31>(), "NonSym Slow 31 failed"); SUCCEED(); }
#endif

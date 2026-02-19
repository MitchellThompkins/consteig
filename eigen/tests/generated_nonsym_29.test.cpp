#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_29) { static_assert(check_single_nonsymmetric_fast<29>(), "NonSym Fast 29 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_29) { static_assert(check_single_nonsymmetric_slow<29>(), "NonSym Slow 29 failed"); SUCCEED(); }
#endif

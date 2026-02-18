#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_38) { static_assert(check_single_nonsymmetric_fast<38>(), "NonSym Fast 38 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_38) { static_assert(check_single_nonsymmetric_slow<38>(), "NonSym Slow 38 failed"); SUCCEED(); }
#endif

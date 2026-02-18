#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_33) { static_assert(check_single_nonsymmetric_fast<33>(), "NonSym Fast 33 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_33) { static_assert(check_single_nonsymmetric_slow<33>(), "NonSym Slow 33 failed"); SUCCEED(); }
#endif

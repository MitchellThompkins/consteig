#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_3) { static_assert(check_single_nonsymmetric_fast<3>(), "NonSym Fast 3 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_3) { static_assert(check_single_nonsymmetric_slow<3>(), "NonSym Slow 3 failed"); SUCCEED(); }
#endif

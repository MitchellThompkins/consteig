#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_6) { static_assert(check_single_nonsymmetric_fast<6>(), "NonSym Fast 6 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_6) { static_assert(check_single_nonsymmetric_slow<6>(), "NonSym Slow 6 failed"); SUCCEED(); }
#endif

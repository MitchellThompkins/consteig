#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_45) { static_assert(check_single_nonsymmetric_fast<45>(), "NonSym Fast 45 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_45) { static_assert(check_single_nonsymmetric_slow<45>(), "NonSym Slow 45 failed"); SUCCEED(); }
#endif

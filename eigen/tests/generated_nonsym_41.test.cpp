#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_41) { static_assert(check_single_nonsymmetric_fast<41>(), "NonSym Fast 41 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_41) { static_assert(check_single_nonsymmetric_slow<41>(), "NonSym Slow 41 failed"); SUCCEED(); }
#endif

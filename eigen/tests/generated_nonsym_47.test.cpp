#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_47) { static_assert(check_single_nonsymmetric_fast<47>(), "NonSym Fast 47 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_47) { static_assert(check_single_nonsymmetric_slow<47>(), "NonSym Slow 47 failed"); SUCCEED(); }
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_37) { static_assert(check_single_nonsymmetric_fast<37>(), "NonSym Fast 37 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_37) { static_assert(check_single_nonsymmetric_slow<37>(), "NonSym Slow 37 failed"); SUCCEED(); }
#endif

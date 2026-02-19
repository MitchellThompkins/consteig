#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_44) { static_assert(check_single_nonsymmetric_fast<44>(), "NonSym Fast 44 failed"); SUCCEED(); }
#else
TEST(generated_tests, nonsym_slow_44) { static_assert(check_single_nonsymmetric_slow<44>(), "NonSym Slow 44 failed"); SUCCEED(); }
#endif

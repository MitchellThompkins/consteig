#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_0) { static_assert(check_single_symmetric_fast<0>(), "Sym Fast 0 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_0) { static_assert(check_single_symmetric_slow<0>(), "Sym Slow 0 failed"); SUCCEED(); }
#endif

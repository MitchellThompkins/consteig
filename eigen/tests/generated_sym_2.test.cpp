#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_2) { static_assert(check_single_symmetric_fast<2>(), "Sym Fast 2 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_2) { static_assert(check_single_symmetric_slow<2>(), "Sym Slow 2 failed"); SUCCEED(); }
#endif

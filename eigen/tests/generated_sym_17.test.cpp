#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_17) { static_assert(check_single_symmetric_fast<17>(), "Sym Fast 17 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_17) { static_assert(check_single_symmetric_slow<17>(), "Sym Slow 17 failed"); SUCCEED(); }
#endif

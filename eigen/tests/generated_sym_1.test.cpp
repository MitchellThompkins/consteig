#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_1) { static_assert(check_single_symmetric_fast<1>(), "Sym Fast 1 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_1) { static_assert(check_single_symmetric_slow<1>(), "Sym Slow 1 failed"); SUCCEED(); }
#endif

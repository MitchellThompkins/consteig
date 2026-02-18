#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_32) { static_assert(check_single_symmetric_fast<32>(), "Sym Fast 32 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_32) { static_assert(check_single_symmetric_slow<32>(), "Sym Slow 32 failed"); SUCCEED(); }
#endif

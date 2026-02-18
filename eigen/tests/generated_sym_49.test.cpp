#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_49) { static_assert(check_single_symmetric_fast<49>(), "Sym Fast 49 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_49) { static_assert(check_single_symmetric_slow<49>(), "Sym Slow 49 failed"); SUCCEED(); }
#endif

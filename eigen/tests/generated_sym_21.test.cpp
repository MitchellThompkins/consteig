#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_21) { static_assert(check_single_symmetric_fast<21>(), "Sym Fast 21 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_21) { static_assert(check_single_symmetric_slow<21>(), "Sym Slow 21 failed"); SUCCEED(); }
#endif

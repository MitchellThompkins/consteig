#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_10) { static_assert(check_single_symmetric_fast<10>(), "Sym Fast 10 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_10) { static_assert(check_single_symmetric_slow<10>(), "Sym Slow 10 failed"); SUCCEED(); }
#endif

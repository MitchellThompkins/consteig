#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_31) { static_assert(check_single_symmetric_fast<31>(), "Sym Fast 31 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_31) { static_assert(check_single_symmetric_slow<31>(), "Sym Slow 31 failed"); SUCCEED(); }
#endif

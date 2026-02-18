#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_4) { static_assert(check_single_symmetric_fast<4>(), "Sym Fast 4 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_4) { static_assert(check_single_symmetric_slow<4>(), "Sym Slow 4 failed"); SUCCEED(); }
#endif

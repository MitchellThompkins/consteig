#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_11) { static_assert(check_single_symmetric_fast<11>(), "Sym Fast 11 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_11) { static_assert(check_single_symmetric_slow<11>(), "Sym Slow 11 failed"); SUCCEED(); }
#endif

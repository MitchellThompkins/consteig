#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_30) { static_assert(check_single_symmetric_fast<30>(), "Sym Fast 30 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_30) { static_assert(check_single_symmetric_slow<30>(), "Sym Slow 30 failed"); SUCCEED(); }
#endif

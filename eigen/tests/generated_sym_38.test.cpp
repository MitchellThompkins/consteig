#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_38) { static_assert(check_single_symmetric_fast<38>(), "Sym Fast 38 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_38) { static_assert(check_single_symmetric_slow<38>(), "Sym Slow 38 failed"); SUCCEED(); }
#endif

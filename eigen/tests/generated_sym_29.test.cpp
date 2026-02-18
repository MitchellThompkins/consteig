#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_29) { static_assert(check_single_symmetric_fast<29>(), "Sym Fast 29 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_29) { static_assert(check_single_symmetric_slow<29>(), "Sym Slow 29 failed"); SUCCEED(); }
#endif

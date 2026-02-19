#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_41) { static_assert(check_single_symmetric_fast<41>(), "Sym Fast 41 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_41) { static_assert(check_single_symmetric_slow<41>(), "Sym Slow 41 failed"); SUCCEED(); }
#endif

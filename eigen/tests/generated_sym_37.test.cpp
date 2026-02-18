#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_37) { static_assert(check_single_symmetric_fast<37>(), "Sym Fast 37 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_37) { static_assert(check_single_symmetric_slow<37>(), "Sym Slow 37 failed"); SUCCEED(); }
#endif

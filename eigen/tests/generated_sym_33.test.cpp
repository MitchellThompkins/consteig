#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_33) { static_assert(check_single_symmetric_fast<33>(), "Sym Fast 33 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_33) { static_assert(check_single_symmetric_slow<33>(), "Sym Slow 33 failed"); SUCCEED(); }
#endif

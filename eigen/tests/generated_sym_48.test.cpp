#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_48) { static_assert(check_single_symmetric_fast<48>(), "Sym Fast 48 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_48) { static_assert(check_single_symmetric_slow<48>(), "Sym Slow 48 failed"); SUCCEED(); }
#endif

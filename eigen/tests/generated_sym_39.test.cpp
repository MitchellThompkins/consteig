#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_39) { static_assert(check_single_symmetric_fast<39>(), "Sym Fast 39 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_39) { static_assert(check_single_symmetric_slow<39>(), "Sym Slow 39 failed"); SUCCEED(); }
#endif

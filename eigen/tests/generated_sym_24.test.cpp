#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_24) { static_assert(check_single_symmetric_fast<24>(), "Sym Fast 24 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_24) { static_assert(check_single_symmetric_slow<24>(), "Sym Slow 24 failed"); SUCCEED(); }
#endif

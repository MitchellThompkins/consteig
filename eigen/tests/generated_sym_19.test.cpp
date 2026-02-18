#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_19) { static_assert(check_single_symmetric_fast<19>(), "Sym Fast 19 failed"); SUCCEED(); }
#else
TEST(generated_tests, sym_slow_19) { static_assert(check_single_symmetric_slow<19>(), "Sym Slow 19 failed"); SUCCEED(); }
#endif

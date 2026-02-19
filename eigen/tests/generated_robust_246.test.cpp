#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_fast_3) { static_assert(check_single_large_jordan_nonsym_fast<3>(), "Test large_jordan_fast_3 failed"); SUCCEED(); }
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_fast_9) { static_assert(check_single_repeated_nonsym_fast<9>(), "Test repeated_fast_9 failed"); SUCCEED(); }
#endif
#endif

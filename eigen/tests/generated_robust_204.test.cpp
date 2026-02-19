#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_fast_2) { static_assert(check_single_companion_nonsym_fast<2>(), "Test companion_fast_2 failed"); SUCCEED(); }
#endif
#endif

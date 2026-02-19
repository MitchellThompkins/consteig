#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_fast_4) { static_assert(check_single_companion_nonsym_fast<4>(), "Test companion_fast_4 failed"); SUCCEED(); }
#endif
#endif

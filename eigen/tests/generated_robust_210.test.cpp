#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_fast_5) { EXPECT_TRUE(check_single_companion_nonsym_fast<5>()) << "Test companion_fast_5 failed"; }
#endif
#endif

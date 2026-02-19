#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_fast_6) { EXPECT_TRUE(check_single_companion_nonsym_fast<6>()) << "Test companion_fast_6 failed"; }
#endif
#endif

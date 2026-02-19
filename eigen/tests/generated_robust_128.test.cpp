#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_defective_fast_4) { EXPECT_TRUE(check_single_nearly_defective_nonsym_fast<4>()) << "Test nearly_defective_fast_4 failed"; }
#endif
#endif

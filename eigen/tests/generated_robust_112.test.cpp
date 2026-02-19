#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_fast_6) { EXPECT_TRUE(check_single_defective_nonsym_fast<6>()) << "Test defective_fast_6 failed"; }
#endif
#endif

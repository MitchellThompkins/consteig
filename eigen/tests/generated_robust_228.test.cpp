#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, graded_fast_4) {
    EXPECT_TRUE(check_single_graded_nonsym_fast<4>()) << "Test graded_fast_4 failed";
}
#endif
#endif

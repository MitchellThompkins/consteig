#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, graded_fast_0) {
    EXPECT_TRUE(check_single_graded_nonsym_fast<0>()) << "Test graded_fast_0 failed";
}
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_fast_0) {
    EXPECT_TRUE(check_single_repeated_nonsym_fast<0>()) << "Test repeated_fast_0 failed";
}
#endif
#endif

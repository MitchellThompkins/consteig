#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_fast_5) {
    EXPECT_TRUE(check_single_defective_nonsym_fast<5>()) << "Test defective_fast_5 failed";
}
#endif
#endif

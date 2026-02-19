#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, graded_slow_2) {
    EXPECT_TRUE(check_single_graded_nonsym_slow<2>()) << "Test graded_slow_2 failed";
}
#endif
#endif

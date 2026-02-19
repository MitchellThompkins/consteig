#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, toeplitz_slow_3) {
    EXPECT_TRUE(check_single_toeplitz_nonsym_slow<3>()) << "Test toeplitz_slow_3 failed";
}
#endif
#endif

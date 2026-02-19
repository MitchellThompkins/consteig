#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_slow_8) {
    EXPECT_TRUE(check_single_companion_nonsym_slow<8>()) << "Test companion_slow_8 failed";
}
#endif
#endif

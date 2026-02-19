#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_slow_7) {
    EXPECT_TRUE(check_single_companion_nonsym_slow<7>()) << "Test companion_slow_7 failed";
}
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_slow_5) { EXPECT_TRUE(check_single_random_non_normal_nonsym_slow<5>()) << "Test random_non_normal_slow_5 failed"; }
#endif
#endif

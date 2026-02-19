#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_slow_0) { EXPECT_TRUE(check_single_random_non_normal_nonsym_slow<0>()) << "Test random_non_normal_slow_0 failed"; }
#endif
#endif

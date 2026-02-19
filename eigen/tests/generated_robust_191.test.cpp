#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_slow_5) { EXPECT_TRUE(check_single_repeated_nonsym_slow<5>()) << "Test repeated_slow_5 failed"; }
#endif
#endif

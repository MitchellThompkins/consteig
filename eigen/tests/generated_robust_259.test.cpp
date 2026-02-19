#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_slow_9) { EXPECT_TRUE(check_single_large_jordan_nonsym_slow<9>()) << "Test large_jordan_slow_9 failed"; }
#endif
#endif

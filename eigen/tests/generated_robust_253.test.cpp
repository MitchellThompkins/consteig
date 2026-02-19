#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, large_jordan_slow_6) { EXPECT_TRUE(check_single_large_jordan_nonsym_slow<6>()) << "Test large_jordan_slow_6 failed"; }
#endif
#endif

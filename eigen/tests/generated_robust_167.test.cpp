#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_slow_3) { EXPECT_TRUE(check_single_clustered_nonsym_slow<3>()) << "Test clustered_slow_3 failed"; }
#endif
#endif

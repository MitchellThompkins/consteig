#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_slow_6) { EXPECT_TRUE(check_single_defective_nonsym_slow<6>()) << "Test defective_slow_6 failed"; }
#endif
#endif

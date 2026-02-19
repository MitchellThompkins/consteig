#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nearly_defective_slow_1) { EXPECT_TRUE(check_single_nearly_defective_nonsym_slow<1>()) << "Test nearly_defective_slow_1 failed"; }
#endif
#endif

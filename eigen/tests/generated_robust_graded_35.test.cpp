#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_10x10_35) { static_assert(check_single_graded_nonsym_10x10<35>(), "Test graded_10x10_35 failed"); SUCCEED(); }
#endif

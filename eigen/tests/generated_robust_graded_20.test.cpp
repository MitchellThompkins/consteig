#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_10x10_20) { static_assert(check_single_graded_nonsym_10x10<20>(), "Test graded_10x10_20 failed"); SUCCEED(); }
#endif

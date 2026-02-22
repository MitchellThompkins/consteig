#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, non_normal_10x10_2) { static_assert(check_single_non_normal_nonsym_10x10<2>(), "Test non_normal_10x10_2 failed"); SUCCEED(); }
#endif

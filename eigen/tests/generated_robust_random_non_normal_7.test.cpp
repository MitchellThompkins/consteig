#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, random_non_normal_10x10_7) { static_assert(check_single_random_non_normal_nonsym_10x10<7>(), "Test random_non_normal_10x10_7 failed"); SUCCEED(); }
#endif

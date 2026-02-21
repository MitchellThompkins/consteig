#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, random_non_normal_8x8_17) { static_assert(check_single_random_non_normal_nonsym_8x8<17>(), "Test random_non_normal_8x8_17 failed"); SUCCEED(); }
#endif

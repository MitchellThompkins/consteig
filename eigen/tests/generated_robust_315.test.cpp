#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_slow_7) { static_assert(check_single_random_non_normal_nonsym_slow<7>(), "Test random_non_normal_slow_7 failed"); SUCCEED(); }
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, companion_10x10_35) { static_assert(check_single_companion_nonsym_10x10<35>(), "Test companion_10x10_35 failed"); SUCCEED(); }
#endif

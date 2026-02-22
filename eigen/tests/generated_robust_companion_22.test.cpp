#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, companion_10x10_22) { static_assert(check_single_companion_nonsym_10x10<22>(), "Test companion_10x10_22 failed"); SUCCEED(); }
#endif

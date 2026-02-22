#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, companion_10x10_40) { static_assert(check_single_companion_nonsym_10x10<40>(), "Test companion_10x10_40 failed"); SUCCEED(); }
#endif

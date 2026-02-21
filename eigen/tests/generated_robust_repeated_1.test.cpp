#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, repeated_8x8_1) { static_assert(check_single_repeated_nonsym_8x8<1>(), "Test repeated_8x8_1 failed"); SUCCEED(); }
#endif

#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_16) { static_assert(check_single_graded_nonsym_8x8<16>(), "Test graded_8x8_16 failed"); SUCCEED(); }

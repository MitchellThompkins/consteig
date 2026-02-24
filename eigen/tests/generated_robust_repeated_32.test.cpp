#include "generated_test_helpers.hpp"
TEST(generated_tests, repeated_8x8_32) { static_assert(check_single_repeated_nonsym_8x8<32>(), "Test repeated_8x8_32 failed"); SUCCEED(); }

#include "generated_test_helpers.hpp"
TEST(generated_tests, toeplitz_8x8_40) { static_assert(check_single_toeplitz_nonsym_8x8<40>(), "Test toeplitz_8x8_40 failed"); SUCCEED(); }

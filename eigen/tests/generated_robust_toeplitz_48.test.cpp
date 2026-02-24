#include "generated_test_helpers.hpp"
TEST(generated_tests, toeplitz_8x8_48) { static_assert(check_single_toeplitz_nonsym_8x8<48>(), "Test toeplitz_8x8_48 failed"); SUCCEED(); }

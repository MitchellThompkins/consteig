#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_15) { static_assert(check_single_random_sym_10x10<15>(), "Test random_10x10_15 failed"); SUCCEED(); }

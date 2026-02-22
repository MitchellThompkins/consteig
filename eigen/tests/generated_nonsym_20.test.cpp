#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_20) { static_assert(check_single_random_nonsym_10x10<20>(), "Test random_10x10_20 failed"); SUCCEED(); }

#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_30) { static_assert(check_single_random_nonsym_10x10<30>(), "Test random_10x10_30 failed"); SUCCEED(); }

#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_21) {
    static_assert(check_single_random_nonsym_8x8<21>(), "Test random_8x8_21 failed");
    SUCCEED();
}

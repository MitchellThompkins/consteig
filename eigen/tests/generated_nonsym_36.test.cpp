#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_36) {
    static_assert(check_single_random_nonsym_8x8<36>(),
                  "Test random_8x8_36 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_9) {
    static_assert(check_single_random_nonsym_8x8<9>(),
                  "Test random_8x8_9 failed");
    SUCCEED();
}

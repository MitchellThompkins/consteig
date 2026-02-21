#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_13) {
    static_assert(check_single_random_nonsym_8x8<13>(),
                  "Test random_8x8_13 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_38) {
    static_assert(check_single_random_nonsym_8x8<38>(),
                  "Test random_8x8_38 failed");
    SUCCEED();
}

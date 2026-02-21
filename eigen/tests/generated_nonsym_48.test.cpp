#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_48) {
    static_assert(check_single_random_nonsym_8x8<48>(),
                  "Test random_8x8_48 failed");
    SUCCEED();
}

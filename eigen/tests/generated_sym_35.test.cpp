#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_35) {
    static_assert(check_single_random_sym_8x8<35>(),
                  "Test random_8x8_35 failed");
    SUCCEED();
}

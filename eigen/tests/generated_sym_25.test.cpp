#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_25) {
    static_assert(check_single_random_sym_8x8<25>(), "Test random_8x8_25 failed");
    SUCCEED();
}

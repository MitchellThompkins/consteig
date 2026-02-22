#include "generated_test_helpers.hpp"
TEST(generated_tests, random_8x8_28)
{
    static_assert(check_single_random_sym_8x8<28>(),
                  "Test random_8x8_28 failed");
    SUCCEED();
}

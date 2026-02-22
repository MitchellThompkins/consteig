#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_14)
{
    static_assert(check_single_random_sym_10x10<14>(),
                  "Test random_10x10_14 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_21)
{
    static_assert(check_single_random_nonsym_10x10<21>(),
                  "Test random_10x10_21 failed");
    SUCCEED();
}

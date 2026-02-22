#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_34)
{
    static_assert(check_single_random_nonsym_10x10<34>(),
                  "Test random_10x10_34 failed");
    SUCCEED();
}

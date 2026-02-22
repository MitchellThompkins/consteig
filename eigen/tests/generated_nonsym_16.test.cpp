#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_16)
{
    static_assert(check_single_random_nonsym_10x10<16>(),
                  "Test random_10x10_16 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_24)
{
    static_assert(check_single_random_nonsym_10x10<24>(),
                  "Test random_10x10_24 failed");
    SUCCEED();
}

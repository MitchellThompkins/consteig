#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_49)
{
    static_assert(check_single_random_nonsym_10x10<49>(),
                  "Test random_10x10_49 failed");
    SUCCEED();
}

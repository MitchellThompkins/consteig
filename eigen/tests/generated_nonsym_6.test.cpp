#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_6)
{
    static_assert(check_single_random_nonsym_10x10<6>(),
                  "Test random_10x10_6 failed");
    SUCCEED();
}

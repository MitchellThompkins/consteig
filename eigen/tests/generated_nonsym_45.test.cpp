#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_45)
{
    static_assert(check_single_random_nonsym_10x10<45>(),
                  "Test random_10x10_45 failed");
    SUCCEED();
}

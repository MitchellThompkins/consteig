#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_23)
{
    static_assert(check_single_random_nonsym_10x10<23>(),
                  "Test random_10x10_23 failed");
    SUCCEED();
}

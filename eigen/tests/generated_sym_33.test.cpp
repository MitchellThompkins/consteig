#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_33)
{
    static_assert(check_single_random_sym_10x10<33>(),
                  "Test random_10x10_33 failed");
    SUCCEED();
}

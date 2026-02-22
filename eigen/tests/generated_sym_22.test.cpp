#include "generated_test_helpers.hpp"
TEST(generated_tests, random_10x10_22)
{
    static_assert(check_single_random_sym_10x10<22>(),
                  "Test random_10x10_22 failed");
    SUCCEED();
}

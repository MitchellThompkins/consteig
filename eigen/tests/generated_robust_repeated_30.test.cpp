#include "generated_test_helpers.hpp"
TEST(generated_tests, repeated_8x8_30)
{
    static_assert(check_single_repeated_nonsym_8x8<30>(),
                  "Test repeated_8x8_30 failed");
    SUCCEED();
}

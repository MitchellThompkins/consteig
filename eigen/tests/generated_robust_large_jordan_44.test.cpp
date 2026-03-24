#include "generated_test_helpers.hpp"
TEST(generated_tests, large_jordan_8x8_44)
{
    static_assert(check_single_large_jordan_nonsym_8x8<44>(),
                  "Test large_jordan_8x8_44 failed");
    SUCCEED();
}

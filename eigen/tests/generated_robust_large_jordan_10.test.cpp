#include "generated_test_helpers.hpp"
TEST(generated_tests, large_jordan_8x8_10)
{
    static_assert(check_single_large_jordan_nonsym_8x8<10>(),
                  "Test large_jordan_8x8_10 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, large_jordan_8x8_37)
{
    static_assert(check_single_large_jordan_nonsym_8x8<37>(),
                  "Test large_jordan_8x8_37 failed");
    SUCCEED();
}

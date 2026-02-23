#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_14)
{
    static_assert(check_single_graded_nonsym_8x8<14>(),
                  "Test graded_8x8_14 failed");
    SUCCEED();
}

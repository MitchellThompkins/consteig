#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_29)
{
    static_assert(check_single_graded_nonsym_8x8<29>(),
                  "Test graded_8x8_29 failed");
    SUCCEED();
}

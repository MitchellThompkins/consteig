#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_25)
{
    static_assert(check_single_graded_nonsym_8x8<25>(),
                  "Test graded_8x8_25 failed");
    SUCCEED();
}

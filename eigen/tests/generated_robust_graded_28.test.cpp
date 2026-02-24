#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_28)
{
    static_assert(check_single_graded_nonsym_8x8<28>(),
                  "Test graded_8x8_28 failed");
    SUCCEED();
}

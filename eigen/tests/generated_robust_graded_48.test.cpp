#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_48)
{
    static_assert(check_single_graded_nonsym_8x8<48>(),
                  "Test graded_8x8_48 failed");
    SUCCEED();
}

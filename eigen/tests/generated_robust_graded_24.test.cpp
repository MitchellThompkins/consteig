#include "generated_test_helpers.hpp"
TEST(generated_tests, graded_8x8_24)
{
    static_assert(check_single_graded_nonsym_8x8<24>(),
                  "Test graded_8x8_24 failed");
    SUCCEED();
}

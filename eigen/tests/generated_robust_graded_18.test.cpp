#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_8x8_18)
{
    static_assert(check_single_graded_nonsym_8x8<18>(),
                  "Test graded_8x8_18 failed");
    SUCCEED();
}
#endif

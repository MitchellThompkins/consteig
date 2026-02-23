#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_8x8_22)
{
    static_assert(check_single_graded_nonsym_8x8<22>(),
                  "Test graded_8x8_22 failed");
    SUCCEED();
}
#endif

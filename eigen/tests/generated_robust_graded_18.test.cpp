#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_10x10_18)
{
    static_assert(check_single_graded_nonsym_10x10<18>(),
                  "Test graded_10x10_18 failed");
    SUCCEED();
}
#endif

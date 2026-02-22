#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, graded_10x10_14)
{
    static_assert(check_single_graded_nonsym_10x10<14>(),
                  "Test graded_10x10_14 failed");
    SUCCEED();
}
#endif

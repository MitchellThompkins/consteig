#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, clustered_8x8_27)
{
    static_assert(check_single_clustered_nonsym_8x8<27>(),
                  "Test clustered_8x8_27 failed");
    SUCCEED();
}
#endif

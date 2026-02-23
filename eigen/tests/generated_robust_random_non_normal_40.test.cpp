#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, random_non_normal_8x8_40)
{
    static_assert(check_single_random_non_normal_nonsym_8x8<40>(),
                  "Test random_non_normal_8x8_40 failed");
    SUCCEED();
}
#endif

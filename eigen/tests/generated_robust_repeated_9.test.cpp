#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, repeated_8x8_9)
{
    static_assert(check_single_repeated_nonsym_8x8<9>(),
                  "Test repeated_8x8_9 failed");
    SUCCEED();
}
#endif

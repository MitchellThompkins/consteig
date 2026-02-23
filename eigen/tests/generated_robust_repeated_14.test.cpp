#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, repeated_8x8_14)
{
    static_assert(check_single_repeated_nonsym_8x8<14>(),
                  "Test repeated_8x8_14 failed");
    SUCCEED();
}
#endif

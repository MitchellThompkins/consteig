#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, repeated_8x8_37)
{
    static_assert(check_single_repeated_nonsym_8x8<37>(),
                  "Test repeated_8x8_37 failed");
    SUCCEED();
}
#endif

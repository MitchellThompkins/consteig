#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, companion_10x10_1)
{
    static_assert(check_single_companion_nonsym_10x10<1>(),
                  "Test companion_10x10_1 failed");
    SUCCEED();
}
#endif

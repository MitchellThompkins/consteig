#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, large_jordan_8x8_28)
{
    static_assert(check_single_large_jordan_nonsym_8x8<28>(),
                  "Test large_jordan_8x8_28 failed");
    SUCCEED();
}
#endif

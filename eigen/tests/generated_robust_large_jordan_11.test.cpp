#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, large_jordan_10x10_11)
{
    static_assert(check_single_large_jordan_nonsym_10x10<11>(),
                  "Test large_jordan_10x10_11 failed");
    SUCCEED();
}
#endif

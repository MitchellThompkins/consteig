#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, large_jordan_10x10_24)
{
    static_assert(check_single_large_jordan_nonsym_10x10<24>(),
                  "Test large_jordan_10x10_24 failed");
    SUCCEED();
}
#endif

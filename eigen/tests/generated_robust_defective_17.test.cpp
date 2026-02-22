#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, defective_10x10_17)
{
    static_assert(check_single_defective_nonsym_10x10<17>(),
                  "Test defective_10x10_17 failed");
    SUCCEED();
}
#endif

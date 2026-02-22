#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, defective_8x8_47)
{
    static_assert(check_single_defective_nonsym_8x8<47>(),
                  "Test defective_8x8_47 failed");
    SUCCEED();
}
#endif

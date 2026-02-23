#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_defective_8x8_48)
{
    static_assert(check_single_nearly_defective_nonsym_8x8<48>(),
                  "Test nearly_defective_8x8_48 failed");
    SUCCEED();
}
#endif

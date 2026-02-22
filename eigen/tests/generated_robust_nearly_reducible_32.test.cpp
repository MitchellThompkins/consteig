#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, nearly_reducible_8x8_32)
{
    static_assert(check_single_nearly_reducible_nonsym_8x8<32>(),
                  "Test nearly_reducible_8x8_32 failed");
    SUCCEED();
}
#endif

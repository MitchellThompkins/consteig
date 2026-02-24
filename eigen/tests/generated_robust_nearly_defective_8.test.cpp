#include "generated_test_helpers.hpp"
TEST(generated_tests, nearly_defective_8x8_8)
{
    static_assert(check_single_nearly_defective_nonsym_8x8<8>(),
                  "Test nearly_defective_8x8_8 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, nearly_defective_8x8_2)
{
    static_assert(check_single_nearly_defective_nonsym_8x8<2>(), "Test nearly_defective_8x8_2 failed");
    SUCCEED();
}

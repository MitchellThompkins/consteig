#include "generated_test_helpers.hpp"
TEST(generated_tests, nearly_reducible_8x8_44)
{
    static_assert(check_single_nearly_reducible_nonsym_8x8<44>(), "Test nearly_reducible_8x8_44 failed");
    SUCCEED();
}

#include "generated_test_helpers.hpp"
TEST(generated_tests, nearly_reducible_8x8_1)
{
    static_assert(check_single_nearly_reducible_nonsym_8x8<1>(),
                  "Test nearly_reducible_8x8_1 failed");
    SUCCEED();
}

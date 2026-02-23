#include "generated_test_helpers.hpp"
TEST(generated_tests, nearly_reducible_8x8_42)
{
    static_assert(check_single_nearly_reducible_nonsym_8x8<42>(),
                  "Test nearly_reducible_8x8_42 failed");
    SUCCEED();
}

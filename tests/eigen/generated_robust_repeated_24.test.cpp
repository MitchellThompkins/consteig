#include "generated_test_helpers.hpp"
TEST(generated_tests, repeated_8x8_24)
{
    static_assert(check_single_repeated_nonsym_8x8<24>(), "Test repeated_8x8_24 failed");
    SUCCEED();
}

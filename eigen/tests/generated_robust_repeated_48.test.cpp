#include "generated_test_helpers.hpp"
TEST(generated_tests, repeated_8x8_48)
{
    static_assert(check_single_repeated_nonsym_8x8<48>(), "Test repeated_8x8_48 failed");
    SUCCEED();
}

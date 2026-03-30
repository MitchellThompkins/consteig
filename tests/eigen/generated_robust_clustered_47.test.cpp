#include "generated_test_helpers.hpp"
TEST(generated_tests, clustered_8x8_47)
{
    static_assert(check_single_clustered_nonsym_8x8<47>(), "Test clustered_8x8_47 failed");
    SUCCEED();
}

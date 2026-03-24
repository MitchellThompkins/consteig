#include "generated_test_helpers.hpp"
TEST(generated_tests, clustered_8x8_32)
{
    static_assert(check_single_clustered_nonsym_8x8<32>(),
                  "Test clustered_8x8_32 failed");
    SUCCEED();
}

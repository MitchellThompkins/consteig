#include "generated_test_helpers.hpp"
TEST(generated_tests, defective_8x8_39)
{
    static_assert(check_single_defective_nonsym_8x8<39>(), "Test defective_8x8_39 failed");
    SUCCEED();
}

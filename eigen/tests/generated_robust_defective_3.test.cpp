#include "generated_test_helpers.hpp"
TEST(generated_tests, defective_8x8_3)
{
    static_assert(check_single_defective_nonsym_8x8<3>(),
                  "Test defective_8x8_3 failed");
    SUCCEED();
}

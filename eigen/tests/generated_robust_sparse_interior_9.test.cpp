#include "generated_test_helpers.hpp"
TEST(generated_tests, sparse_interior_8x8_9)
{
    static_assert(check_single_sparse_interior_nonsym_8x8<9>(), "Test sparse_interior_8x8_9 failed");
    SUCCEED();
}

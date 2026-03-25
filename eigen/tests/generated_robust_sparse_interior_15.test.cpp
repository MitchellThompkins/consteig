#include "generated_test_helpers.hpp"
TEST(generated_tests, sparse_interior_8x8_15)
{
    static_assert(check_single_sparse_interior_nonsym_8x8<15>(), "Test sparse_interior_8x8_15 failed");
    SUCCEED();
}

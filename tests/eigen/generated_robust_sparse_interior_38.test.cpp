#include "generated_test_helpers.hpp"
TEST(generated_tests, sparse_interior_8x8_38)
{
    static_assert(check_single_sparse_interior_nonsym_8x8<38>(), "Test sparse_interior_8x8_38 failed");
    SUCCEED();
}

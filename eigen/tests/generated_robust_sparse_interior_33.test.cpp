#include "generated_test_helpers.hpp"
TEST(generated_tests, sparse_interior_8x8_33)
{
    static_assert(check_single_sparse_interior_nonsym_8x8<33>(), "Test sparse_interior_8x8_33 failed");
    SUCCEED();
}

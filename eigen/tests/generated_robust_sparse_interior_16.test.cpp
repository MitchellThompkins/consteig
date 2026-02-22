#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, sparse_interior_8x8_16)
{
    static_assert(check_single_sparse_interior_nonsym_8x8<16>(),
                  "Test sparse_interior_8x8_16 failed");
    SUCCEED();
}
#endif

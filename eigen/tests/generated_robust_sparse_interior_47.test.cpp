#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, sparse_interior_10x10_47)
{
    static_assert(check_single_sparse_interior_nonsym_10x10<47>(),
                  "Test sparse_interior_10x10_47 failed");
    SUCCEED();
}
#endif

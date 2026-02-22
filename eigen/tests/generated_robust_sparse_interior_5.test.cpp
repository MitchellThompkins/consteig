#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, sparse_interior_10x10_5)
{
    static_assert(check_single_sparse_interior_nonsym_10x10<5>(),
                  "Test sparse_interior_10x10_5 failed");
    SUCCEED();
}
#endif

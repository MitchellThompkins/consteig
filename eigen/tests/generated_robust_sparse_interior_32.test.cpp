#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
TEST(generated_tests, sparse_interior_8x8_32) {
    static_assert(check_single_sparse_interior_nonsym_8x8<32>(), "Test sparse_interior_8x8_32 failed");
    SUCCEED();
}
#endif

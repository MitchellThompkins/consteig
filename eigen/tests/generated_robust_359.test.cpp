#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_slow_9) {
    EXPECT_TRUE(check_single_sparse_interior_nonsym_slow<9>()) << "Test sparse_interior_slow_9 failed";
}
#endif
#endif

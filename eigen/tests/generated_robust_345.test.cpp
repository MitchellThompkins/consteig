#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_slow_2) { EXPECT_TRUE(check_single_sparse_interior_nonsym_slow<2>()) << "Test sparse_interior_slow_2 failed"; }
#endif
#endif

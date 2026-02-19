#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_fast_4) { EXPECT_TRUE(check_single_sparse_interior_nonsym_fast<4>()) << "Test sparse_interior_fast_4 failed"; }
#endif
#endif

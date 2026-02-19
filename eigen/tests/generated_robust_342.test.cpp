#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_fast_1) { EXPECT_TRUE(check_single_sparse_interior_nonsym_fast<1>()) << "Test sparse_interior_fast_1 failed"; }
#endif
#endif

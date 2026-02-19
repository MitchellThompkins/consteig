#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_fast_6) { EXPECT_TRUE(check_single_sparse_interior_nonsym_fast<6>()) << "Test sparse_interior_fast_6 failed"; }
#endif
#endif

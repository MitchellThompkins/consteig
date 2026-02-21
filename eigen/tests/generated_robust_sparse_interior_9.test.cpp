#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_fast_9) { static_assert(check_single_sparse_interior_nonsym_fast<9>(), "Test sparse_interior_fast_9 failed"); SUCCEED(); }
#else
TEST(generated_tests, sparse_interior_slow_9) { static_assert(check_single_sparse_interior_nonsym_slow<9>(), "Test sparse_interior_slow_9 failed"); SUCCEED(); }
#endif
#endif

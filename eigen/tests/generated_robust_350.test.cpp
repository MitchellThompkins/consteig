#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_fast_5) { static_assert(check_single_sparse_interior_nonsym_fast<5>(), "Test sparse_interior_fast_5 failed"); SUCCEED(); }
#endif
#endif

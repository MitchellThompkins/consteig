#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sparse_interior_fast_7) {
    static_assert(check_single_sparse_interior_nonsym_fast<7>(), "Test sparse_interior_fast_7 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sparse_interior_slow_7) {
    static_assert(check_single_sparse_interior_nonsym_slow<7>(), "Test sparse_interior_slow_7 failed");
    SUCCEED();
}
#endif
#endif

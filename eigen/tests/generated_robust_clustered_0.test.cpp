#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_fast_0) {
    static_assert(check_single_clustered_nonsym_fast<0>(), "Test clustered_fast_0 failed");
    SUCCEED();
}
#else
TEST(generated_tests, clustered_slow_0) {
    static_assert(check_single_clustered_nonsym_slow<0>(), "Test clustered_slow_0 failed");
    SUCCEED();
}
#endif
#endif

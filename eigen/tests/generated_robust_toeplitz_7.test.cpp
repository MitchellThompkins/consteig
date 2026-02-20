#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, toeplitz_fast_7) {
    static_assert(check_single_toeplitz_nonsym_fast<7>(), "Test toeplitz_fast_7 failed");
    SUCCEED();
}
#else
TEST(generated_tests, toeplitz_slow_7) {
    static_assert(check_single_toeplitz_nonsym_slow<7>(), "Test toeplitz_slow_7 failed");
    SUCCEED();
}
#endif
#endif

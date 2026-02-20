#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, toeplitz_fast_4) {
    static_assert(check_single_toeplitz_nonsym_fast<4>(), "Test toeplitz_fast_4 failed");
    SUCCEED();
}
#endif
#endif

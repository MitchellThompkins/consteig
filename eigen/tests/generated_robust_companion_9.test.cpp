#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_fast_9) {
    static_assert(check_single_companion_nonsym_fast<9>(), "Test companion_fast_9 failed");
    SUCCEED();
}
#else
TEST(generated_tests, companion_slow_9) {
    static_assert(check_single_companion_nonsym_slow<9>(), "Test companion_slow_9 failed");
    SUCCEED();
}
#endif
#endif

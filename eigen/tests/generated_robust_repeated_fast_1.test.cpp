#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_fast_1) {
    static_assert(check_single_repeated_nonsym_fast<1>(), "Test repeated_fast_1 failed");
    SUCCEED();
}
#endif
#endif

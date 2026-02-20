#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, repeated_fast_5) {
    static_assert(check_single_repeated_nonsym_fast<5>(), "Test repeated_fast_5 failed");
    SUCCEED();
}
#endif
#endif

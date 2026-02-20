#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_fast_1) {
    static_assert(check_single_defective_nonsym_fast<1>(), "Test defective_fast_1 failed");
    SUCCEED();
}
#else
TEST(generated_tests, defective_slow_1) {
    static_assert(check_single_defective_nonsym_slow<1>(), "Test defective_slow_1 failed");
    SUCCEED();
}
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, defective_fast_8) {
    static_assert(check_single_defective_nonsym_fast<8>(), "Test defective_fast_8 failed");
    SUCCEED();
}
#else
TEST(generated_tests, defective_slow_8) {
    static_assert(check_single_defective_nonsym_slow<8>(), "Test defective_slow_8 failed");
    SUCCEED();
}
#endif
#endif

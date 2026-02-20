#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, non_normal_fast_3) {
    static_assert(check_single_non_normal_nonsym_fast<3>(), "Test non_normal_fast_3 failed");
    SUCCEED();
}
#endif
#endif

#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_non_normal_fast_8) {
    static_assert(check_single_random_non_normal_nonsym_fast<8>(), "Test random_non_normal_fast_8 failed");
    SUCCEED();
}
#endif
#endif

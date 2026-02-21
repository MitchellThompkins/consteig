#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_23) {
    static_assert(check_single_random_nonsym_fast<23>(), "Test random_fast_23 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_23) {
    static_assert(check_single_random_nonsym_slow<23>(), "Test random_slow_23 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_18) {
    static_assert(check_single_random_nonsym_fast<18>(), "Test random_fast_18 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_18) {
    static_assert(check_single_random_nonsym_slow<18>(), "Test random_slow_18 failed");
    SUCCEED();
}
#endif

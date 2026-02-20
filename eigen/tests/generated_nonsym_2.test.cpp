#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_2) {
    static_assert(check_single_random_nonsym_fast<2>(), "Test random_fast_2 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_2) {
    static_assert(check_single_random_nonsym_slow<2>(), "Test random_slow_2 failed");
    SUCCEED();
}
#endif

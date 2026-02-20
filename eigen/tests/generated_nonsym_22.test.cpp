#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_22) {
    static_assert(check_single_random_nonsym_fast<22>(), "Test random_fast_22 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_22) {
    static_assert(check_single_random_nonsym_slow<22>(), "Test random_slow_22 failed");
    SUCCEED();
}
#endif

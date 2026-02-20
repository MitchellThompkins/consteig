#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_34) {
    static_assert(check_single_random_nonsym_fast<34>(), "Test random_fast_34 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_34) {
    static_assert(check_single_random_nonsym_slow<34>(), "Test random_slow_34 failed");
    SUCCEED();
}
#endif

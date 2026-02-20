#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_45) {
    static_assert(check_single_random_nonsym_fast<45>(), "Test random_fast_45 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_45) {
    static_assert(check_single_random_nonsym_slow<45>(), "Test random_slow_45 failed");
    SUCCEED();
}
#endif

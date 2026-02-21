#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_13) {
    static_assert(check_single_random_sym_fast<13>(), "Test random_fast_13 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_13) {
    static_assert(check_single_random_sym_slow<13>(), "Test random_slow_13 failed");
    SUCCEED();
}
#endif

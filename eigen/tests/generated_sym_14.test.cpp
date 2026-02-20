#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_14) {
    static_assert(check_single_random_sym_fast<14>(), "Test random_fast_14 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_14) {
    static_assert(check_single_random_sym_slow<14>(), "Test random_slow_14 failed");
    SUCCEED();
}
#endif

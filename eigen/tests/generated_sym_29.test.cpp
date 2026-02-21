#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_29) {
    static_assert(check_single_random_sym_fast<29>(), "Test random_fast_29 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_29) {
    static_assert(check_single_random_sym_slow<29>(), "Test random_slow_29 failed");
    SUCCEED();
}
#endif

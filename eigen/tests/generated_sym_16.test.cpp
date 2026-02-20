#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_16) {
    static_assert(check_single_random_sym_fast<16>(), "Test random_fast_16 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_16) {
    static_assert(check_single_random_sym_slow<16>(), "Test random_slow_16 failed");
    SUCCEED();
}
#endif

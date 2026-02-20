#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_31) {
    static_assert(check_single_random_sym_fast<31>(), "Test random_fast_31 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_31) {
    static_assert(check_single_random_sym_slow<31>(), "Test random_slow_31 failed");
    SUCCEED();
}
#endif

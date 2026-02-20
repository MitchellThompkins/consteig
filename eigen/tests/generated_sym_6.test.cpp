#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_6) {
    static_assert(check_single_random_sym_fast<6>(), "Test random_fast_6 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_6) {
    static_assert(check_single_random_sym_slow<6>(), "Test random_slow_6 failed");
    SUCCEED();
}
#endif

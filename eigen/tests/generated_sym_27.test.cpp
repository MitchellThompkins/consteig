#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_27) {
    static_assert(check_single_random_sym_fast<27>(), "Test random_fast_27 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_27) {
    static_assert(check_single_random_sym_slow<27>(), "Test random_slow_27 failed");
    SUCCEED();
}
#endif

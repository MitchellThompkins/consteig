#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_25) {
    static_assert(check_single_random_sym_fast<25>(), "Test random_fast_25 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_25) {
    static_assert(check_single_random_sym_slow<25>(), "Test random_slow_25 failed");
    SUCCEED();
}
#endif

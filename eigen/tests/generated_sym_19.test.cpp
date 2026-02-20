#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_19) {
    static_assert(check_single_random_sym_fast<19>(), "Test random_fast_19 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_19) {
    static_assert(check_single_random_sym_slow<19>(), "Test random_slow_19 failed");
    SUCCEED();
}
#endif

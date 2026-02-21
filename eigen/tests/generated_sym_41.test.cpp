#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_41) {
    static_assert(check_single_random_sym_fast<41>(), "Test random_fast_41 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_41) {
    static_assert(check_single_random_sym_slow<41>(), "Test random_slow_41 failed");
    SUCCEED();
}
#endif

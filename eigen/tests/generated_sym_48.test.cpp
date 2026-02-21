#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_48) {
    static_assert(check_single_random_sym_fast<48>(), "Test random_fast_48 failed");
    SUCCEED();
}
#else
TEST(generated_tests, random_slow_48) {
    static_assert(check_single_random_sym_slow<48>(), "Test random_slow_48 failed");
    SUCCEED();
}
#endif

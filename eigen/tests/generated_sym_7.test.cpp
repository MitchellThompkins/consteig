#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_7) {
    static_assert(check_single_random_sym_fast<7>(), "Test random_fast_7 failed");
    SUCCEED();
}
#endif

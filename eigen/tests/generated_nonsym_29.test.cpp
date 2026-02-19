#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_29) {
    static_assert(check_single_random_nonsym_fast<29>(), "Test random_fast_29 failed");
    SUCCEED();
}
#endif

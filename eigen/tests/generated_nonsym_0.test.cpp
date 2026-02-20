#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_0) {
    static_assert(check_single_random_nonsym_fast<0>(), "Test random_fast_0 failed");
    SUCCEED();
}
#endif

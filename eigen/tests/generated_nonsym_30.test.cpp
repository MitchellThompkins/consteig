#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_30) {
    static_assert(check_single_random_nonsym_fast<30>(), "Test random_fast_30 failed");
    SUCCEED();
}
#endif

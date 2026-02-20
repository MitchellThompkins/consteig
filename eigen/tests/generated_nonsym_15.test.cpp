#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_15) {
    static_assert(check_single_random_nonsym_fast<15>(), "Test random_fast_15 failed");
    SUCCEED();
}
#endif

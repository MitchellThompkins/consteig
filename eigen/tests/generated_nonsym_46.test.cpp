#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_fast_46) {
    static_assert(check_single_random_nonsym_fast<46>(), "Test random_fast_46 failed");
    SUCCEED();
}
#endif

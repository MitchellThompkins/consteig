#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_3) {
    static_assert(check_single_random_nonsym_slow<3>(), "Test random_slow_3 failed");
    SUCCEED();
}
#endif

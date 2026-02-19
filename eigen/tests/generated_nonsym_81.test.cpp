#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_31) {
    static_assert(check_single_random_nonsym_slow<31>(), "Test random_slow_31 failed");
    SUCCEED();
}
#endif

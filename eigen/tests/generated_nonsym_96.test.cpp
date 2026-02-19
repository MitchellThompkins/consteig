#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_46) {
    static_assert(check_single_random_nonsym_slow<46>(), "Test random_slow_46 failed");
    SUCCEED();
}
#endif

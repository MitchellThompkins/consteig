#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_24) {
    static_assert(check_single_random_nonsym_slow<24>(), "Test random_slow_24 failed");
    SUCCEED();
}
#endif

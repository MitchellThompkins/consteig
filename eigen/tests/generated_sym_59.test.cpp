#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_9) {
    static_assert(check_single_random_sym_slow<9>(), "Test random_slow_9 failed");
    SUCCEED();
}
#endif

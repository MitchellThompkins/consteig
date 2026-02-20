#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_28) {
    static_assert(check_single_random_sym_slow<28>(), "Test random_slow_28 failed");
    SUCCEED();
}
#endif

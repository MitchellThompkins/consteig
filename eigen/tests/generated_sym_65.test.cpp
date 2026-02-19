#include "generated_test_helpers.hpp"
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, random_slow_15) {
    static_assert(check_single_random_sym_slow<15>(), "Test random_slow_15 failed");
    SUCCEED();
}
#endif

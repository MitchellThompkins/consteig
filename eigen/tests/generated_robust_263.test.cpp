#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, toeplitz_slow_1) {
    static_assert(check_single_toeplitz_nonsym_slow<1>(), "Test toeplitz_slow_1 failed");
    SUCCEED();
}
#endif
#endif

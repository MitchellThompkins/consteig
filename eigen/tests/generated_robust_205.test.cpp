#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, companion_slow_2) {
    static_assert(check_single_companion_nonsym_slow<2>(), "Test companion_slow_2 failed");
    SUCCEED();
}
#endif
#endif

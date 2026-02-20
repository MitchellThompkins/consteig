#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, graded_slow_0) {
    static_assert(check_single_graded_nonsym_slow<0>(), "Test graded_slow_0 failed");
    SUCCEED();
}
#endif
#endif

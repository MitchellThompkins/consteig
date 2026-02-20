#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifdef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_slow_2) {
    static_assert(check_single_clustered_nonsym_slow<2>(), "Test clustered_slow_2 failed");
    SUCCEED();
}
#endif
#endif

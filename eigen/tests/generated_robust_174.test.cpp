#include "generated_test_helpers.hpp"
#ifdef ENABLE_ROBUSTNESS
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, clustered_fast_7) {
    static_assert(check_single_clustered_nonsym_fast<7>(), "Test clustered_fast_7 failed");
    SUCCEED();
}
#endif
#endif

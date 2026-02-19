#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_35) {
    static_assert(check_single_nonsymmetric_fast<35>(), "NonSym Fast 35 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_35) {
    static_assert(check_single_nonsymmetric_slow<35>(), "NonSym Slow 35 failed");
    SUCCEED();
}
#endif

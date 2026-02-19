#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_9) {
    static_assert(check_single_nonsymmetric_fast<9>(), "NonSym Fast 9 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_9) {
    static_assert(check_single_nonsymmetric_slow<9>(), "NonSym Slow 9 failed");
    SUCCEED();
}
#endif

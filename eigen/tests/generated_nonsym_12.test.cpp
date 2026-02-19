#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_12) {
    static_assert(check_single_nonsymmetric_fast<12>(), "NonSym Fast 12 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_12) {
    static_assert(check_single_nonsymmetric_slow<12>(), "NonSym Slow 12 failed");
    SUCCEED();
}
#endif

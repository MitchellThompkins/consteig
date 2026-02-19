#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_14) {
    static_assert(check_single_nonsymmetric_fast<14>(), "NonSym Fast 14 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_14) {
    static_assert(check_single_nonsymmetric_slow<14>(), "NonSym Slow 14 failed");
    SUCCEED();
}
#endif

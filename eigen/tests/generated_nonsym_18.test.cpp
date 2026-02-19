#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_18) {
    static_assert(check_single_nonsymmetric_fast<18>(), "NonSym Fast 18 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_18) {
    static_assert(check_single_nonsymmetric_slow<18>(), "NonSym Slow 18 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_16) {
    static_assert(check_single_nonsymmetric_fast<16>(), "NonSym Fast 16 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_16) {
    static_assert(check_single_nonsymmetric_slow<16>(), "NonSym Slow 16 failed");
    SUCCEED();
}
#endif

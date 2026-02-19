#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_32) {
    static_assert(check_single_nonsymmetric_fast<32>(), "NonSym Fast 32 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_32) {
    static_assert(check_single_nonsymmetric_slow<32>(), "NonSym Slow 32 failed");
    SUCCEED();
}
#endif

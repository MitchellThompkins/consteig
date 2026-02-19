#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_7) {
    static_assert(check_single_nonsymmetric_fast<7>(), "NonSym Fast 7 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_7) {
    static_assert(check_single_nonsymmetric_slow<7>(), "NonSym Slow 7 failed");
    SUCCEED();
}
#endif

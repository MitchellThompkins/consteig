#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_5) {
    static_assert(check_single_nonsymmetric_fast<5>(), "NonSym Fast 5 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_5) {
    static_assert(check_single_nonsymmetric_slow<5>(), "NonSym Slow 5 failed");
    SUCCEED();
}
#endif

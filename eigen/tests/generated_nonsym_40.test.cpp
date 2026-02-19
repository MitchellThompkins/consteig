#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_40) {
    static_assert(check_single_nonsymmetric_fast<40>(), "NonSym Fast 40 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_40) {
    static_assert(check_single_nonsymmetric_slow<40>(), "NonSym Slow 40 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_42) {
    static_assert(check_single_nonsymmetric_fast<42>(), "NonSym Fast 42 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_42) {
    static_assert(check_single_nonsymmetric_slow<42>(), "NonSym Slow 42 failed");
    SUCCEED();
}
#endif

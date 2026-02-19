#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_30) {
    static_assert(check_single_nonsymmetric_fast<30>(), "NonSym Fast 30 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_30) {
    static_assert(check_single_nonsymmetric_slow<30>(), "NonSym Slow 30 failed");
    SUCCEED();
}
#endif

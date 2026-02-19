#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_27) {
    static_assert(check_single_nonsymmetric_fast<27>(), "NonSym Fast 27 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_27) {
    static_assert(check_single_nonsymmetric_slow<27>(), "NonSym Slow 27 failed");
    SUCCEED();
}
#endif

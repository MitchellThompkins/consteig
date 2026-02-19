#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_43) {
    static_assert(check_single_nonsymmetric_fast<43>(), "NonSym Fast 43 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_43) {
    static_assert(check_single_nonsymmetric_slow<43>(), "NonSym Slow 43 failed");
    SUCCEED();
}
#endif

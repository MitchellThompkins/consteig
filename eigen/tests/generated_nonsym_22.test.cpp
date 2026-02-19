#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_22) {
    static_assert(check_single_nonsymmetric_fast<22>(), "NonSym Fast 22 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_22) {
    static_assert(check_single_nonsymmetric_slow<22>(), "NonSym Slow 22 failed");
    SUCCEED();
}
#endif

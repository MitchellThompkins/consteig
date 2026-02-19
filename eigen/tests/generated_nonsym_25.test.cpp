#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_25) {
    static_assert(check_single_nonsymmetric_fast<25>(), "NonSym Fast 25 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_25) {
    static_assert(check_single_nonsymmetric_slow<25>(), "NonSym Slow 25 failed");
    SUCCEED();
}
#endif

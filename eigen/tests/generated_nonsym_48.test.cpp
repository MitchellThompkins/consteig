#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_48) {
    static_assert(check_single_nonsymmetric_fast<48>(), "NonSym Fast 48 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_48) {
    static_assert(check_single_nonsymmetric_slow<48>(), "NonSym Slow 48 failed");
    SUCCEED();
}
#endif

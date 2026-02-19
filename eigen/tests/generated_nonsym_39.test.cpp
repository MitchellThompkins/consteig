#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_39) {
    static_assert(check_single_nonsymmetric_fast<39>(), "NonSym Fast 39 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_39) {
    static_assert(check_single_nonsymmetric_slow<39>(), "NonSym Slow 39 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_19) {
    static_assert(check_single_nonsymmetric_fast<19>(), "NonSym Fast 19 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_19) {
    static_assert(check_single_nonsymmetric_slow<19>(), "NonSym Slow 19 failed");
    SUCCEED();
}
#endif

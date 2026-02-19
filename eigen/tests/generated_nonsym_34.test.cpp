#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_34) {
    static_assert(check_single_nonsymmetric_fast<34>(), "NonSym Fast 34 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_34) {
    static_assert(check_single_nonsymmetric_slow<34>(), "NonSym Slow 34 failed");
    SUCCEED();
}
#endif

#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_24) {
    static_assert(check_single_nonsymmetric_fast<24>(), "NonSym Fast 24 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_24) {
    static_assert(check_single_nonsymmetric_slow<24>(), "NonSym Slow 24 failed");
    SUCCEED();
}
#endif

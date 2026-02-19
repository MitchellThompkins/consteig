#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, nonsym_fast_46) {
    static_assert(check_single_nonsymmetric_fast<46>(), "NonSym Fast 46 failed");
    SUCCEED();
}
#else
TEST(generated_tests, nonsym_slow_46) {
    static_assert(check_single_nonsymmetric_slow<46>(), "NonSym Slow 46 failed");
    SUCCEED();
}
#endif

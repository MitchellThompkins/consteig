#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_36) {
    static_assert(check_single_symmetric_fast<36>(), "Sym Fast 36 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_36) {
    static_assert(check_single_symmetric_slow<36>(), "Sym Slow 36 failed");
    SUCCEED();
}
#endif

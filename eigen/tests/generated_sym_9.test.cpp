#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_9) {
    static_assert(check_single_symmetric_fast<9>(), "Sym Fast 9 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_9) {
    static_assert(check_single_symmetric_slow<9>(), "Sym Slow 9 failed");
    SUCCEED();
}
#endif

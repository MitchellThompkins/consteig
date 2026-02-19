#include "generated_test_helpers.hpp"
#ifndef CONSTEIG_SLOW_TESTS
TEST(generated_tests, sym_fast_35) {
    static_assert(check_single_symmetric_fast<35>(), "Sym Fast 35 failed");
    SUCCEED();
}
#else
TEST(generated_tests, sym_slow_35) {
    static_assert(check_single_symmetric_slow<35>(), "Sym Slow 35 failed");
    SUCCEED();
}
#endif
